/*
 * A018529  Divisors of 578.
 * https://oeis.org/A018529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth divisor of 578 in ascending order
   (OEIS A018529, offset 1) */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* indices < 1 are undefined */

    /* Build the divisor list once */
    static seq_t divisors[6];
    static int ready = 0;
    if (!ready)
    {
        size_t idx = 0;
        /* generate all 2^a * 17^b, a = 0..1, b = 0..2 */
        for (uint64_t a = 0; a <= 1; ++a)
        {
            for (uint64_t b = 0; b <= 2; ++b)
            {
                seq_t v = 1;
                if (a) v *= (seq_t)2;      /* multiply by 2 if a==1 */

                /* multiply by 17^b */
                if (b)
                {
                    seq_t pow = 1;
                    for (uint64_t k = 0; k < b; ++k) pow *= (seq_t)17;
                    v *= pow;
                }
                divisors[idx++] = v;
            }
        }

        /* sort the 6 values in ascending order */
        for (size_t i = 0; i < 6; ++i)
            for (size_t j = i + 1; j < 6; ++j)
                if (divisors[j] < divisors[i])
                {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }

        ready = 1;
    }

    if (n > 6) return (seq_t)0;  /* no further divisors */

    return divisors[n - 1];
}
