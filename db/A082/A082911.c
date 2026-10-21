/*
 * A082911  a(n) = prime(n+pi(n)) - prime(n) = A000040(n+A000720(n)) -
 *          A000040(n).
 * https://oeis.org/A082911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n+pi(n)) - prime(n) (offset 1)
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/estimate_limit_for_idx.h"

/* Static cache for primes */
static seq_t *prime_arr   = NULL;   /* array of primes (seq_t type) */
static size_t prime_cap   = 0;      /* allocated capacity for prime_arr */
static size_t prime_cnt   = 0;      /* number of primes currently stored */
static size_t sieve_limit = 0;      /* current maximal number sieved */

/* Build a sieve up to 'limit' and fill prime_arr with all primes ≤ limit */
static void ensure_sieve_limit(size_t limit)
{
    if (limit <= sieve_limit) return;        /* already sufficient */

    sieve_limit = limit;

    /* Allocate and zero the sieve array */
    uint8_t *is_composite = (uint8_t*)calloc(limit + 1, 1);
    if (!is_composite) return;                /* out of memory – unlikely for our limits */

    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;

    /* Resize prime_arr if needed */
    if (prime_cap < limit * 2) {
        prime_cap = limit * 2;
        prime_arr = (seq_t*)realloc(prime_arr, prime_cap * sizeof(seq_t));
    }
    prime_cnt = 0;

    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                prime_arr = (seq_t*)realloc(prime_arr, prime_cap * sizeof(seq_t));
            }
            prime_arr[prime_cnt++] = (seq_t)i;
        }

    free(is_composite);
}

/* Count of primes ≤ n (π(n)). Uses binary search on prime_arr. */
static size_t pi_of_n(idx_t n)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_arr[mid] <= (seq_t)n) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* ---------------------------------------------------------------
 *  Compute a(n) = prime(n+π(n)) – prime(n)
 *  offset: 1 – a(1) corresponds to index 1
 * --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for 0, undefined by offset */

    /* Ensure primes are available up to the n‑th prime */
    size_t need_idx = n;
    size_t limit = estimate_limit_for_idx(need_idx);
    ensure_sieve_limit(limit);
    while (prime_cnt < need_idx) {           /* safety for very large n */
        limit = sieve_limit * 2 + 1;
        ensure_sieve_limit(limit);
    }

    /* Z = prime(n) */
    seq_t Z = prime_arr[n - 1];

    /* π(n) : number of primes ≤ n */
    size_t pi_n = pi_of_n(n);

    /* Need prime at index n + π(n) */
    size_t idx_needed = n + pi_n;
    limit = estimate_limit_for_idx(idx_needed);
    ensure_sieve_limit(limit);
    while (prime_cnt < idx_needed) {           /* safety for large indices */
        limit = sieve_limit * 2 + 1;
        ensure_sieve_limit(limit);
    }

    seq_t Y = prime_arr[idx_needed - 1];

    /* Return the difference */
    return Y - Z;
}
