/*
 * A161660  a(1)=1; a(n) = largest multiple of {number of divisors of a(n-1)}
 *          that is <= n.
 * https://oeis.org/A161660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest multiple of τ(a(n-1)) not exceeding n, with
 *            a(1)=1 (τ(m) = number of divisors of m)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---------- internal helpers ---------------------------------------------- */
static seq_t *cache = NULL;        /* memoised a(n) values                    */
static idx_t cache_cap = 0;        /* current cache size                      */
static uint16_t *divisors = NULL;  /* number of divisors for each integer     */
static idx_t div_cap = 0;          /* current maximum precomputed divisor count */

static void ensure_cache(idx_t n)
{
    if (cache == NULL || cache_cap <= n)
    {
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; ++i) new_cache[i] = 0;
        free(cache);
        cache = new_cache;
        cache_cap = n;
    }
}

static void ensure_divisors(idx_t n)
{
    if (divisors == NULL || div_cap <= n)
    {
        uint16_t *new_div = (uint16_t *)malloc((n + 1) * sizeof(uint16_t));
        for (idx_t i = 0; i <= n; ++i) new_div[i] = 0;
        for (idx_t d = 1; d <= n; ++d)
        {
            for (idx_t j = d; j <= n; j += d)
                new_div[j] += 1;
        }
        free(divisors);
        divisors = new_div;
        div_cap = n;
    }
}

/* ---------- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – callers should not use n==0 */
    ensure_cache(n);
    ensure_divisors(n);

    if (cache[n] != 0) return cache[n];  /* memoised result already known */

    if (n == 1) {                        /* initial value */
        cache[1] = 1;
        return 1;
    }

    seq_t prev = a(n - 1);                /* recursive call – memoised */
    seq_t d = divisors[(idx_t)prev];      /* τ(a(n‑1))            */
    seq_t result = ((seq_t)n / d) * d;    /* largest multiple ≤ n */

    cache[n] = result;
    return result;
}
