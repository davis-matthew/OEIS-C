/*
 * A223567  Number of nX6 0..1 arrays with antidiagonals unimodal.
 * https://oeis.org/A223567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1}^{min(n,6)-1} (1 + i*(i+1)/2)^2 * (1 +
 *            min(n,6)*(min(n,6)+1)/2)^(max(n,6)-min(n,6)+1) a(n) = 22*a(n-1)
 *            for n>5 (The formula above expands to the recurrence for n>5.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed values (0‑based) */
static idx_t cache_len = 0;   /* number of values currently cached */

/* expand the cache to at least size n (1‑based) */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_len) return;
    size_t needed = (size_t)n;
    size_t new_cap = cache_len ? cache_len : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    /* cache_len will be updated later */
}

/* fast binary exponentiation of seq_t by idx_t exponent (non‑negative) */
static inline seq_t
seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 – not used but safe */

    ensure_cache(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i > 6) {
            /* recurrence for n > 6 */
            val = cache[i-2] * (seq_t)22;
        } else {
            idx_t m = i;                  /* m = min(i,6) */
            seq_t prod = (seq_t)1;
            for (idx_t j = 1; j < m; ++j) {
                seq_t t = (seq_t)1 + (seq_t)j * (j + 1) / 2;
                prod *= t * t;            /* square and multiply */
            }
            seq_t base = (seq_t)1 + (seq_t)m * (m + 1) / 2;
            idx_t exp  = 7 - i;           /* (max(i,6)-m+1) simplifies to 7-i for i≤6 */
            seq_t pow_term = seq_pow(base, exp);
            val = prod * pow_term;
        }
        cache[i-1] = val;
    }
    cache_len = n;
    return cache[n-1];
}
