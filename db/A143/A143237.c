/*
 * A143237  Triangle read by rows, T(n, k) = A000203(n)*A000203(k), for n >=
 *          1, 1 <= k <= n.
 * https://oeis.org/A143237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sigma(i) * sigma(j), where i is the row index * such that
 *            i*(i-1)/2 < n <= i*(i+1)/2 (1‑based), * and j = n - i*(i-1)/2.
 *            Here sigma(k) = A000203(k)
 * Range:     u8 n <= 52, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Compute the sum of divisors of n (sigma(n)) by trial division. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = (idx_t)1; ((seq_t)d * d) <= (seq_t)n; ++d) {
        if (n % d == (idx_t)0) {
            sum += (seq_t)d;
            idx_t e = n / d;
            if (e != d) {
                sum += (seq_t)e;
            }
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find the row i: smallest i with i*(i+1)/2 >= n. */
    idx_t i = (idx_t)1;
    seq_t tri = ((seq_t)i * (i + (idx_t)1)) / (seq_t)2;
    while (tri < (seq_t)n) {
        ++i;
        tri = ((seq_t)i * (i + (idx_t)1)) / (seq_t)2;
    }

    /* Compute column j = n - i*(i-1)/2. */
    seq_t before = ((seq_t)(i - (idx_t)1) * i) / (seq_t)2;
    idx_t j = n - (idx_t)before;   /* j is guaranteed to fit in idx_t */

    seq_t sigma_i = sigma_of(i);
    seq_t sigma_j = sigma_of(j);
    return sigma_i * sigma_j;
}
