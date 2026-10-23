/*
 * A174433  Triangle read by rows: T(n,k) = prime(n) mod A001223(k), where
 *          A001223 are differences between consecutive primes.
 * https://oeis.org/A174433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = prime(n) mod (prime(k+1)-prime(k)) a(n) is the triangle
 *            read by rows, i.e. the linearization of T with row r containing
 *            r elements, r ≥ 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps a static cache of primes; the
    required prime index for a given n is r (row number) and we
    also need the prime gaps up to index r+1.  All indices are
    handled with the idx_t type (uint64_t).  This code works
    for any seq_t that supports +,-,*,/,% and comparisons,
    as required by the build system. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* static cache of primes: prime_list[0] = 2 */
static uint64_t *prime_list = NULL;
static size_t   prime_cap  = 0;
static size_t   prime_cnt  = 0;

/* make sure we have at least `need` primes (need >= 1) */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    /* grow capacity */
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    if (new_cap < need) new_cap = need;

    uint64_t *new_arr = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (!new_arr) abort();

    if (prime_list) {
        memcpy(new_arr, prime_list, prime_cnt * sizeof(uint64_t));
        free(prime_list);
    }
    prime_list = new_arr;
    prime_cap  = new_cap;

    /* find next primes */
    uint64_t candidate = prime_cnt ? prime_list[prime_cnt - 1] + 1ULL : 2ULL;
    while (prime_cnt < need) {
        /* trial division by stored primes */
        bool is_prime = true;
        if (candidate < 2) candidate = 2;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* find row r such that triangular(r-1) < n <= triangular(r) */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* column k (1‑based) */
    idx_t k = n - ((r - 1) * r) / 2;   /* k ∈ [1, r] */

    /* we need primes up to index r+1 (for the gap at k) */
    ensure_primes((size_t)(r + 1));

    /* prime(r) */
    seq_t prime_row = (seq_t)prime_list[r - 1];

    /* gap at position k: prime(k+1)-prime(k) */
    seq_t gap = (seq_t)(prime_list[k] - prime_list[k - 1]);

    return prime_row % gap;
}
