/*
 * A217704  Smallest number k > n for which sigma(k-n) = sigma(k+n).
 * https://oeis.org/A217704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 6299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217704
   a(n) = smallest integer k > n such that σ(k-n) = σ(k+n)
   where σ(m) is the sum‑of‑divisors function.
   Computation is performed by direct trial of successive k,
   using a lightweight O(√m) algorithm for σ(m). */
#include <stdbool.h>

/* helper: sum of divisors of a 64‑bit integer */
static seq_t sigma_u64(idx_t m)
{
    if (m == 0) return 0;            /* not needed for this sequence */
    seq_t sum = 1;                    /* divisor 1 */
    for (idx_t i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t j = m / i;
            sum += (seq_t)i;
            if (j != i) sum += (seq_t)j;
        }
    }
    if (m > 1) sum += (seq_t)m;        /* include m itself */
    return sum;
}

seq_t a(idx_t n)
{
    /* search for the minimal k > n with equal σ(k-n) and σ(k+n) */
    idx_t k = n + 1;                   /* start just above n */
    while (1) {
        seq_t sigma1 = sigma_u64(k - n);   /* σ(k – n) */
        seq_t sigma2 = sigma_u64(k + n);   /* σ(k + n) */
        if (sigma1 == sigma2) {
            return (seq_t)k;               /* found the smallest k */
        }
        ++k;
    }
}
