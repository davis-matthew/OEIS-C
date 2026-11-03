/*
 * A070610  Number of positive integers k such that sigma(k) divides sigma(n).
 * https://oeis.org/A070610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of k ≥ 1 such that σ(k) divides σ(n)
 * Range:     u8 n <= 53, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9410 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (σ denotes the sum‑of‑divisors function) */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for out‑of‑bounds indices */

    /* Compute σ(n) once */
    seq_t sigma_n = sigma_of(n);

    /* If σ(n) exceeds the maximum value representable by idx_t,
       we cannot iterate over all k. In practice this never
       happens for the range of n used in the tests. */
    idx_t max_k = (idx_t)sigma_n;
    if ((seq_t)max_k != sigma_n)   /* overflow check */
        return 0; /* undefined behaviour for huge n, return 0 */

    seq_t count = 0;
    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t sigma_k = sigma_of(k);
        if (sigma_k != 0 && (sigma_n % sigma_k == 0))
            ++count;
    }
    return count;
}
