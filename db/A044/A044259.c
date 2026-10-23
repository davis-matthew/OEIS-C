/*
 * A044259  Numbers n such that string 0,8 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: nth integer whose base‑9 representation contains the substring
   "08" but whose predecessor does not. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined at offset 0; return 0 as a safe guard */
        return 0;

    idx_t count = 0;          /* number of terms found so far          */
    seq_t m = 1;              /* current integer being examined        */
    bool prevHas08 = false;   /* does (m-1) have the pattern "08"?     */

    for (;; /* infinite loop; will break via return */)
    {
        /* Check if the current number m contains the substring "08"
           in its base‑9 representation.  We process the digits from
           least significant to most significant, keeping track of the
           previous digit so we can detect the pattern locally. */
        bool has08 = false;
        bool firstDigit = true;
        seq_t prevDigit = 0;   /* placeholder; defined before use */
        seq_t x = m;

        while (x > 0)          // while there are still digits left
        {
            seq_t curDigit = x % 9;   // current least significant digit

            if (!firstDigit)          // not the first (least significant)
            {
                if (prevDigit == 8 && curDigit == 0)
                {
                    has08 = true;
                    break;
                }
            }

            firstDigit = false;
            prevDigit = curDigit;
            x = x / 9;
        }

        /* If m itself satisfies the property and m-1 does not,
           we have found the next term in the sequence. */
        if (has08 && !prevHas08)
        {
            ++count;
            if (count == n)
                return m;
        }

        prevHas08 = has08;

        /* Advance to the next integer: m = m + 1 */
        m = m + 1;
    }
}
