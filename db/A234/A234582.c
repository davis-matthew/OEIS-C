/*
 * A234582  Let S_n = 0 followed by base-7 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the digit at position n in the infinite string formed by
 *            concatenating, for k = 0,1,2,…, the string “0’’ followed by the
 *            base‑7 representation of k reversed. The nth term a(n) is the
 *            digit (0‑9) at index n of that string (0‑based)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t idx = 0;       /* current block index k */
    idx_t len = 0;       /* cumulative length of processed blocks */

    while (1)
    {
        /* determine number of digits of idx in base 7 */
        idx_t t = idx;
        size_t d = 0;
        if (t == 0)
            d = 1;
        else
        {
            while (t != 0)
            {
                ++d;
                t /= 7;
            }
        }

        idx_t block_len = (idx_t)d + 1;   /* 0 + base‑7 digits */

        if (len + block_len > n)          /* the desired digit lies here */
        {
            idx_t offset = n - len;       /* 0‑based position inside this block */
            if (offset == 0)              /* the leading 0 */
                return 0;

            /* compute base‑7 digits of idx, msb→lsb */
            seq_t digits[64];
            t = idx;
            for (size_t i = d; i > 0; --i)
            {
                digits[i - 1] = (seq_t)(t % 7);
                t /= 7;
            }

            size_t pos = d - (size_t)offset;   /* position in msb order */
            return digits[pos];
        }

        len += block_len;
        ++idx;
    }
}
