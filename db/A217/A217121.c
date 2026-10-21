/*
 * A217121  Irregular triangle read by rows in which row n lists the positive
 *          values k such that there are k-1 primes in the open interval
 *          (n-k,n+k).
 * https://oeis.org/A217121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   a(n) is the nth element of the irregular triangle read by rows
 *            where row n lists positive values k such that there are k‑1
 *            primes in the open interval (n-k, n+k). The triangle is read row
 *            by row to form the sequence. This implementation builds a prime
 *            table on demand and caches sequence terms. It...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---- Prime utilities ---------------------------------------------------- */
static idx_t *prime_list = NULL;
static size_t   prime_cnt  = 0;

/* Generate a fresh list of all primes ≤ lim.  This routine is called
   each time the requested limit increases, but the growth in lim is
   only by a small constant per row, so the total cost is modest
   (≈50 M operations for n=10 000). */
static void ensure_primes_limit(idx_t lim)
{
    if (lim <= prime_cnt) return;            /* has at least lim primes? */

    /* First count primes using a simple sieve. */
    uint8_t *sieve = (uint8_t *)calloc((size_t)lim + 1, 1);
    if (!sieve) return;                     /* allocation failure – leave
                                                state unchanged */

    if (lim >= 2) {
        sieve[0] = sieve[1] = 1;             /* 0 and 1 not prime */
        for (idx_t p = 2; p * p <= lim; ++p) {
            if (!sieve[p]) {
                for (idx_t m = p * p; m <= lim; m += p) {
                    sieve[m] = 1;
                }
            }
        }
    }

    /* Count primes to size the array. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (!sieve[i]) ++cnt;

    /* Build the prime list. */
    idx_t *new_list = (idx_t *)calloc(cnt, sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= lim; ++i)
        if (!sieve[i]) new_list[pos++] = i;

    free(prime_list);
    free(sieve);

    prime_list = new_list;
    prime_cnt  = cnt;
}

/* Number of primes ≤ x.  Binary search on the list built by
   ensure_primes_limit. */
static size_t pi_upto(idx_t x)
{
    if (x < 2 || prime_cnt == 0) return 0;
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x) lo = mid + 1;
        else                      hi = mid;
    }
    return lo;
}

/* ---- Sequence cache ----------------------------------------------------- */
static seq_t *term_cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_row  = 1;                /* next row to generate */

/* Append a new term to the cache, resizing if necessary. */
static void cache_append(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                  /* allocation failure – leave
                                                cache unchanged */
        term_cache = tmp;
        cache_cap  = new_cap;
    }
    term_cache[cache_len++] = v;
}

/* ---- Main function ------------------------------------------------------ */
static const idx_t MAX_K = 200;             /* safety bound for k */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n<1 */

    /* Expand cache until we have the desired term. */
    while (cache_len < n) {
        idx_t row  = next_row++;            /* current row number */

        /* Ensure we can count primes up to the largest needed bound. */
        ensure_primes_limit(row + MAX_K + 10);

        /* Examine k = 1 .. MAX_K.  No early break – some solutions
           may appear after a larger k has yielded a larger count. */
        for (idx_t k = 1; k <= MAX_K; ++k) {
            int64_t low_signed  = (int64_t)row - (int64_t)k;
            int64_t high_signed = (int64_t)row + (int64_t)k;

            idx_t low  = (low_signed > 0) ? (idx_t)low_signed : 0;
            idx_t high = (high_signed > 0) ? (idx_t)high_signed : 0;

            /* Count primes in the open interval (low, high). */
            size_t cnt = pi_upto(high - 1) - pi_upto(low);

            if (cnt == k - 1)
                cache_append((seq_t)k);
        }
    }
    return term_cache[n - 1];
}
