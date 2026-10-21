/*
 * A053605  Total multiplicity of the eigenvalue 0 in the spectra of the
 *          n^(n-2) labeled trees on n vertices.
 * https://oeis.org/A053605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^(n-1) - 2 * Σ_{m=2..n} (-1)^m * n^(n-m) * m^(m-2) *
 *            binomial(n-1,m-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* compute a single value of the sequence */
static seq_t compute_a(idx_t n)
{
    seq_t ans = 0;
    if (n == 1) {
        return 1;
    }

    /* pre‑compute powers of n :  n^0 , n^1 , … , n^(n-1) */
    seq_t *pow_n = (seq_t *)malloc(sizeof(seq_t) * n);
    pow_n[0] = 1;
    for (idx_t i = 1; i < n; ++i) {
        pow_n[i] = pow_n[i - 1] * (seq_t) n;
    }

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* main summation */
    for (idx_t m = 2; m <= n; ++m) {
        /* term = n^(n-m) * m^(m-2) * binomial(n-1, m-1) */
        seq_t term = pow_n[n - m];
        term = term * big_pow((seq_t) m, (seq_t) m - 2);
        term = term * binomial(n - 1, m - 1);

        if ((m & 1) == 0)          /* even m → (+1) */
            sum_pos += term;
        else                       /* odd m  → (-1) */
            sum_neg += term;
    }

    /* a(n) = n^(n-1) + 2*sum_neg - 2*sum_pos */
    ans = pow_n[n - 1];
    ans = ans - (sum_pos * 2);
    ans = ans + (sum_neg * 2);

    free(pow_n);
    return ans;
}

/* public interface required by the test harness */
seq_t a(idx_t n)
{
    /* cache computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0) {          /* undefined for 0, return 0 */
        return 0;
    }

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;          /* a(1) */
        cache_cap = 1;
    }

    if ((size_t) n > cache_cap) {
        /* grow cache to accommodate up to n */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t) n);
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            cache[i - 1] = compute_a(i);
        }
        cache_cap = n;
    }

    return cache[n - 1];
}
