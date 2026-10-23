/*
 * A009209  Expansion of e.g.f.: exp(sin(x))/exp(x).
 * https://oeis.org/A009209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..⌊(n-1)/2⌋} (-1)^k * binomial(n-1, 2*k) *
 *            a(n-2*k-1) for n ≥ 1 (OEIS A009209 – expansion of e.g.f.:
 *            exp(sin(x))/exp(x))
 * Range:     i8 n <= 8, i16 n <= 12, i32 n <= 18, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for already evaluated terms */
static seq_t *cache      = NULL;   /* pointer to sequence array */
static size_t cache_cap   = 0;     /* allocated size of cache (in terms) */
static idx_t   cache_len   = 0;    /* number of terms currently stored */

/* Ensure the cache can hold at least (n+1) terms. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    /* cast is needed for C++ where realloc returns void* */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Grow cache as necessary. */
    ensure_capacity(n);

    /* Compute any terms that have not yet been cached. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = 1;
        }
        else
        {
            idx_t max_k = (i - 1) / 2;
            seq_t sum  = 0;

            /* Compute binomial coefficients C(i-1, m) incrementally. */
            seq_t curr_bin = 1;          /* C(i-1, 0) */
            for (idx_t m = 1; m <= 2 * max_k; ++m)
            {
                /* C(i-1, m) = C(i-1, m-1) * (i-m) / m */
                curr_bin = curr_bin * (seq_t)(i - m) / (seq_t)m;

                /* Only even m contribute to the sum. */
                if ((m & 1) == 0)
                {
                    idx_t k    = m >> 1;                     /* k = m/2 */
                    seq_t term = curr_bin * cache[i - 2 * k - 1];
                    if (k & 1)
                        sum -= term;                 /* (-1)^k = -1 when k odd */
                    else
                        sum += term;
                }
            }
            cache[i] = sum;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
