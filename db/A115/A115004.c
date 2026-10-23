/*
 * A115004  a(n) = Sum_{i=1..n, j=1..n, gcd(i,j)=1} (n+1-i)*(n+1-j).
 * https://oeis.org/A115004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n^2 + Σ_{i=2..n} (n+1-i)*(2*n+2-i)*φ(i)
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 409, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t max_n = 0;
static seq_t *phi = NULL;

/* Build Euler's totient table up to n (inclusive).  If a bigger table
   is needed, the old one is freed.  The computation is O(n log log n). */
static void build_phi(idx_t n)
{
    seq_t *new_phi = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!new_phi) return;                    /* allocation failure */

    /* initialise phi[i] = i */
    for (idx_t i = 0; i <= n; ++i)
        new_phi[i] = (seq_t)i;

    uint8_t *is_prime = (uint8_t*)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (!is_prime) {
        free(new_phi);
        return;
    }

    if (n >= 0) is_prime[0] = 0;
    if (n >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i <= n; ++i)
        is_prime[i] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_prime[i]) continue;
        for (idx_t j = i; j <= n; j += i) {
            new_phi[j] -= new_phi[j] / (seq_t)i;
            if (j > i) is_prime[j] = 0;   /* mark composite */
        }
    }

    free(is_prime);

    if (phi) free(phi);
    phi = new_phi;
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for n<1 */

    if (n > max_n) build_phi(n);

    seq_t result = (seq_t)n * (seq_t)n;      /* n^2 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t f1 = (seq_t)(n + 1 - i);          /* (n+1-i) */
        seq_t f2 = (seq_t)(2 * n + 2 - i);      /* (2n+2-i) */
        seq_t term = f1 * f2;                   /* (n+1-i)*(2n+2-i) */
        result += term * phi[i];
    }

    return result;
}
