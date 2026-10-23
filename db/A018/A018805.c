/*
 * A018805  Number of elements in the set {(x,y): 1 <= x,y <= n, gcd(x,y)=1}.
 * https://oeis.org/A018805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2 * Sum_{k=1..n} phi(k) - 1 where phi(k) is Euler's
 *            totient function
 * Range:     u8 n <= 20, u16 n <= 328, u32 n <= 8973, u64 n <= 8973,
 *            u128 n <= 8973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *phi_cache = NULL;
static idx_t phi_alloc = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is large enough */
    if (n + 1 > phi_alloc) {
        if (phi_cache) free((void *)phi_cache);
        phi_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!phi_cache) return 0; /* allocation failed */
        phi_alloc = n + 1;
    }

    /* Initialize phi(i) = i */
    for (idx_t i = 0; i <= n; ++i)
        phi_cache[i] = (seq_t)i;

    /* Compute phi by Euler's sieve */
    for (idx_t p = 2; p <= n; ++p) {
        if (phi_cache[p] == (seq_t)p) { /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                phi_cache[j] = (phi_cache[j] / (seq_t)p) * ((seq_t)p - (seq_t)1);
            }
        }
    }

    /* Sum phi(1..n) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += phi_cache[i];

    /* Compute a(n) = 2*sum - 1 */
    return sum * (seq_t)2 - (seq_t)1;
}
