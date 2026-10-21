/*
 * A069518  Geometric mean of digits = 4 and digits are in nondecreasing
 *          order.
 * https://oeis.org/A069518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 65, u64 n <= 346,
 *            u128 n <= 2126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate OEIS sequence A069518.
   The sequence consists of all positive integers whose decimal digits are
   nondecreasing and whose geometric mean of the digits equals 4.
   The digits must therefore belong to {1,2,4,8} and, if the number has
   k digits, the product of the digits must be 4^k.  The function enumerates
   all such numbers in increasing numerical order and returns the one
   corresponding to the requested index n (1‑based). */

/* Note: Implementation uses only integer arithmetic, no floating point,
   and is valid for both C11 and C++17.  The type seq_t is an unsigned
   integer type defined by the build; it may be fixed‑width (uint8_t…
   uint64_t) or arbitrary‑precision.  The type idx_t is an unsigned 64‑bit
   integer. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1, return 0 just in case */

    idx_t count = 0;        /* number of terms found so far */

    for (int k = 1; ; ++k)          /* k = number of digits of the term */
    {
        int z_max = k / 3;          /* maximum possible number of 1's  (exponent 0) */

        for (int z = z_max; z >= 0; --z)            /* count of 1's (exponent 0)  */
        {
            int max_o = (k - 3 * z) / 2;           /* count of 2's (exponent 1)  */
            for (int o = max_o; o >= 0; --o)        /* count of 2's           */
            {
                int e = o + 2 * z;                 /* count of 8's (exponent 3) */
                int t = k - 3 * z - 2 * o;         /* count of 4's (exponent 2) */
                if (t < 0)                         /* safety check (should not fail) */
                    continue;

                /* Build the number: digits are 1 (z), 2 (o), 4 (t), 8 (e). */
                seq_t val = 0;
                for (int i = 0; i < z; ++i) val = val * 10 + 1;
                for (int i = 0; i < o; ++i) val = val * 10 + 2;
                for (int i = 0; i < t; ++i) val = val * 10 + 4;
                for (int i = 0; i < e; ++i) val = val * 10 + 8;

                ++count;
                if (count == n)
                    return val;
            }
        }
    }
}
