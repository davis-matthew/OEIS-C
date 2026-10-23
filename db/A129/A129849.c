/*
 * A129849  Add two consecutive terms : the result shares at least one digit
 *          with one of the terms.
 * https://oeis.org/A129849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1, a(2)=9. For n >= 3, a(n) is the smallest integer m >
 *            a(n-1) such that (a(n-1) + m) shares at least one decimal digit
 *            with either a(n-1) or m
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static inline unsigned digit_mask_seq(seq_t x)
{
    unsigned mask = 0;
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 10);
        mask |= 1U << d;
        x /= 10;
    }
    return mask;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset protection – value unused */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;

    seq_t prev = (seq_t)9;     /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cand = prev + 1;
        unsigned mask_prev = digit_mask_seq(prev);

        while (true)
        {
            seq_t sum = prev + cand;

            unsigned mask_cand = digit_mask_seq(cand);
            unsigned mask_sum  = digit_mask_seq(sum);

            if ((mask_sum & mask_prev) || (mask_sum & mask_cand))
                break;

            cand++;
        }
        prev = cand;   /* shift for next iteration */
    }
    return prev;
}
