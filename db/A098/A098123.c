/*
 * A098123  Number of compositions of n with equal number of even and odd
 *          parts.
 * https://oeis.org/A098123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=floor(n/3)..floor(n/2)}
 *            C(2*n-4*k,n-2*k)*C(n-1-k,2*n-4*k-1)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 71, u128 n <= 138,
 *            big any n
 * Helpers:   binom, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n < 5) {
        static const seq_t base[5] = {1, 0, 0, 2, 0};
        return base[n];
    }

    /* Compute the summation */
    idx_t low  = n / 3;          /* floor(n/3) */
    idx_t high = n / 2;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = low; k <= high; ++k) {
        /* First binomial C(2*n-4*k, n-2*k) */
        idx_t a1 = 2 * n - 4 * k;
        idx_t b1 = n - 2 * k;
        seq_t term1 = binomial(a1, b1);

        /* Second binomial C(n-1-k, 2*n-4*k-1) */
        idx_t tmp = 2 * n - 4 * k;   /* we know tmp >= 0 for k <= n/2 */
        if (tmp == 0) continue;     /* would make the second binomial involve a negative index */

        idx_t a2 = n - 1 - k;
        idx_t b2 = tmp - 1;
        if (b2 > a2) continue;      /* binomial is 0 for b2 > a2 */

        seq_t term2 = binomial(a2, b2);

        sum += term1 * term2;
    }
    return sum;
}
