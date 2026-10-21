/*
 * A056161  Solutions (value of x) of Diophantine equation 2*x^2 + 3*x + 2 =
 *          r^2.
 * https://oeis.org/A056161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 34*a(n-2) - 34*a(n-3) - a(n-4) + a(n-5) initial
 *            values: a(0)=2, a(1)=7, a(2)=94, a(3)=263, a(4)=3218
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A056161 for any non‑negative index n. */
#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_cap = 0;          /* allocated capacity */
static size_t cache_len = 0;          /* number of terms stored (>= cache_cap if full) */

/* Ensure the cache has space for at least 'needed' terms. */
static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 5;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();        /* Out of memory; terminate. */
    cache = tmp;
    cache_cap = newcap;
}

/* Initialise the cache with the first five terms. */
static void init_cache(void)
{
    if (cache_len > 0) return;          /* already initialised */
    ensure_cap(5);
    cache[0] = (seq_t)2;
    cache[1] = (seq_t)7;
    cache[2] = (seq_t)94;
    cache[3] = (seq_t)263;
    cache[4] = (seq_t)3218;
    cache_len = 5;
}

/* Compute all terms up to and including index n, using the recurrence. */
static void compute_up_to(idx_t n)
{
    init_cache();                       /* guarantees cache_len >= 5 */
    size_t need = (size_t)n + 1;        /* number of terms required */
    if (cache_len >= need) return;      /* already computed enough */

    ensure_cap(need);

    for (size_t i = cache_len; i < need; ++i) {
        /* a(i) = a(i-1) + 34*a(i-2) - 34*a(i-3) - a(i-4) + a(i-5) */
        seq_t res = cache[i-1];
        res += cache[i-2] * (seq_t)34;
        res -= cache[i-3] * (seq_t)34;
        res -= cache[i-4];
        res += cache[i-5];
        cache[i] = res;
    }
    cache_len = need;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
