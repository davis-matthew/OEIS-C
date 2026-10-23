/*
 * A179067  Orders of consecutive clusters of twin primes.
 * https://oeis.org/A179067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A179067 – orders of consecutive clusters of twin primes.
 *  The values are obtained by:
 *  1. Sieve of Eratosthenes up to a limit that guarantees at least n runs.
 *  2. Extract the 1‑based indices of lower primes in twin pairs.
 *  3. Scan these indices; a maximal run of successive indices that differ
 *     exactly by 2 corresponds to a cluster of successive twin pairs.
 *  4. The length of such a run is the order k for that cluster.
 *
 *  No closed formula is known; all operations used are integer
 *  arithmetic that is safe for any seq_t (8,16,32,64,128 bits or arbitrary
 *  precision) and any idx_t (uint64_t).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* -------------  Helper to grow a dynamic array  ----------------- */
static void grow_if_needed(void **array, size_t *cap, size_t *len, size_t elem_size)
{
    if (*len >= *cap) {
        *cap = (*cap == 0) ? 16 : (*cap << 1);
        *array = realloc(*array, *cap * elem_size);
    }
}

/* -------------  Static state for memoisation  ------------------ */
static idx_t     sieve_limit   = 0;   /* current prime sieve limit        */
static idx_t   *primes        = NULL;/* array of primes (idx_t)          */
static size_t    prime_cnt     = 0;   /* number of primes                  */
static seq_t   *orders_cache  = NULL;/* computed run lengths (seq_t)      */
static size_t    orders_len    = 0;   /* number of entries cached          */

/* -------------  Re‑sieve and recompute orders  ----------------- */
static void recompute_orders(void)
{
    /* free previous data */
    free(primes);
    free(orders_cache);
    primes = NULL;
    orders_cache = NULL;
    prime_cnt = 0;
    orders_len = 0;

    /* --- 1‑prime sieve ------------------------------------------------- */
    unsigned char *is_prime = (unsigned char*)calloc((size_t)sieve_limit + 3, 1);
    if (!is_prime) exit(1);
    for (idx_t i = 2; i <= sieve_limit + 2; ++i) is_prime[i] = 1;
    for (idx_t p = 2; p * p <= sieve_limit + 2; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= sieve_limit + 2; j += p)
                is_prime[j] = 0;

    /* --- collect prime numbers ---------------------------------------- */
    size_t cap = 0;
    size_t cnt = 0;
    grow_if_needed((void**)&primes, &cap, &cnt, sizeof(idx_t));
    for (idx_t i = 2; i <= sieve_limit + 2; ++i)
        if (is_prime[i]) {
            grow_if_needed((void**)&primes, &cap, &cnt, sizeof(idx_t));
            primes[cnt++] = i;
        }
    prime_cnt = cnt;
    free(is_prime);

    /* --- identify leading indices of twin primes --------------------- */
    idx_t *lead = NULL;
    size_t lead_cap = 0, lead_cnt = 0;
    grow_if_needed((void**)&lead, &lead_cap, &lead_cnt, sizeof(idx_t));
    for (size_t i = 0; i + 1 < prime_cnt; ++i)
        if (primes[i + 1] - primes[i] == 2) {
            grow_if_needed((void**)&lead, &lead_cap, &lead_cnt, sizeof(idx_t));
            /* 1‑based index of the lower prime in the twin pair */
            lead[lead_cnt++] = (idx_t)(i + 1);
        }

    /* --- compute maximal runs of consecutive leading indices + 2 ----- */
    size_t seq_cap = 0;
    seq_t *seq = NULL;
    size_t seq_cnt = 0;
    if (lead_cnt) {
        idx_t run_len = 1;
        for (size_t i = 1; i < lead_cnt; ++i) {
            if (lead[i] - lead[i - 1] == 2)
                ++run_len;
            else {
                grow_if_needed((void**)&seq, &seq_cap, &seq_cnt, sizeof(seq_t));
                seq[seq_cnt++] = (seq_t)run_len;
                run_len = 1;
            }
        }
        /* last run */
        grow_if_needed((void**)&seq, &seq_cap, &seq_cnt, sizeof(seq_t));
        seq[seq_cnt++] = (seq_t)run_len;
    }

    free(lead);

    orders_cache = seq;
    orders_len   = seq_cnt;
}

/* -------------  Ensure that we have at least n orders computed ----- */
static void ensure_n_orders(idx_t n)
{
    if (sieve_limit == 0) {
        /* start with a moderate limit */
        sieve_limit = 2000000;
        recompute_orders();
    }
    while (orders_len < n) {
        /* not enough runs – enlarge sieve limit (e.g. *2) */
        sieve_limit <<= 1;
        recompute_orders();
    }
}

/* -------------  Public sequence function --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1, return 0 for n < 1 */
    ensure_n_orders(n);
    return orders_cache[(size_t)(n - 1)];
}
