/*
 * A159048  Primes of the form m*(m+1)/2 + 4.
 * https://oeis.org/A159048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime of the form m*(m+1)/2 + 4, with offset 1
 *            (so a(1) = 5)
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root: largest x with x*x <= n. */
static seq_t isqrt_seq(seq_t n)
{
    seq_t lo = 0, hi = n;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Trial‑division primality test for seq_t values. */
static bool is_prime_seq_t(seq_t n)
{
    if (n < 2)
        return false;
    if (n == 2 || n == 3)
        return true;
    if ((n & 1) == 0)
        return false;

    seq_t lim = isqrt_seq(n);
    for (seq_t i = 3; i <= lim; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

/* Cached computation of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static state for out‑of‑order calls */
    static seq_t *cache   = NULL;
    static idx_t cached_len = 0;   /* number of primes cached */
    static idx_t cached_cap = 0;   /* allocated capacity */
    static idx_t next_m    = 1;    /* next m to test */

    /* allocate / grow cache as required */
    if (cached_len < n && cached_cap == 0) {
        cached_cap = n < 8 ? 8 : n;  /* initial cap */
        cache = (seq_t *)realloc(NULL, cached_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }
    while (cached_len < n) {
        if (cached_len >= cached_cap) {
            cached_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
        }

        seq_t cand = (seq_t)next_m * ((seq_t)next_m + 1u) / 2u + 4u;
        if (is_prime_seq_t(cand))
            cache[cached_len++] = cand;
        ++next_m;
    }
    return cache[n - 1];
}
