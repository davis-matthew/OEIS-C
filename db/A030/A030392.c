/*
 * A030392  Length of n-th run of digit 0 in A030386.
 * https://oeis.org/A030392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the (n‑1)-th run of zeros in the base‑4
 *            concatenation of the natural numbers, with a(1)=1. The sequence
 *            is indexed so that a(1) is the first run. This code generates
 *            the base‑4 digits of successive integers, scans them for
 *            consecutive zeros, and returns the required length. It wo...
 * Range:     u8 n <= 761, u16 n <= 761, u32 n <= 761, u64 n <= 761,
 *            u128 n <= 761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: a(1) = 1 (the first run is the single 0 that
       appears in the representation of 4). */
    if (n == 1)
        return (seq_t)1;

    /* We need the (n‑1)-th run in the enumeration that starts
       counting from the first run found in the stream. */
    const idx_t target_run = n - 1;
    idx_t run_cnt = 0;

    /* Walk through natural numbers, generating their base‑4 digits
       and looking for runs of consecutive zeros. */
    for (idx_t num = 1; ; ++num)
    {
        idx_t t = num;
        uint8_t digits[64];
        idx_t sz = 0;

        /* Convert num to base 4 (least significant digit first). */
        while (t > 0)
        {
            digits[sz++] = t & 3;   /* t % 4 */
            t >>= 2;               /* t / 4 */
        }

        /* Scan the digits for zero runs. */
        for (idx_t i = 0; i < sz; )
        {
            if (digits[i] == 0)
            {
                seq_t len = 0;
                while (i < sz && digits[i] == 0)
                {
                    ++len;
                    ++i;
                }
                ++run_cnt;
                if (run_cnt == target_run)
                    return len;
            }
            else
            {
                ++i;
            }
        }
    }
}
