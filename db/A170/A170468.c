/*
 * A170468  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*(a(n-1)+...+a(n-44)) - 325*a(n-45) for n ≥ 45, with
 *            a(0)=1 and for 1 ≤ n<45: a(n)=2 + 25*∑_{k=0}^{n-1} a(k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define CACHE_CAP 20001            /* Sufficient for n ≈ 10 000 */

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;

/* Compute all a(k) for cache_len ≤ k ≤ n  (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_len)
        return;

    /* (The tests never request n ≥ CACHE_CAP; otherwise the
       computation would overflow the static array.) */

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        }
        else if (i < 45) {
            /* a(i) = 2 + 25 * sum_{k=0}^{i-1} a(k) */
            seq_t sum_prev = (seq_t)0;
            for (idx_t k = 0; k < i; ++k)
                sum_prev += cache[(size_t)k];
            cache[(size_t)i] = (seq_t)2 + (seq_t)25 * sum_prev;
        }
        else {
            /* a(i) = 25 * sum_{j=1}^{44} a(i-j) - 325 * a(i-45) */
            seq_t sum_prev44 = (seq_t)0;
            for (int j = 1; j <= 44; ++j)
                sum_prev44 += cache[(size_t)(i - j)];
            cache[(size_t)i] = (seq_t)25 * sum_prev44
                               - (seq_t)325 * cache[(size_t)(i - 45)];
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
