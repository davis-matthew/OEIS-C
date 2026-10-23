/*
 * A175966  Complement of A175965(n), where A175965(n) = the lexicographically
 *          earliest sequence with first differences as increasing sequence of
 *          noncomposites A008578.
 * https://oeis.org/A175966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175966: the n-th positive integer that is NOT a partial sum of the
   non‑composite numbers (1, primes).  The sequence is the complement of
   A175965, where A175965 is the lexicographically earliest sequence
   whose first differences are the increasing sequence of non‑composites
   (1,2,3,5,7,11,13,…).  This implementation builds the list of
   non‑composites once (sieve up to LIMIT) and then scans the natural
   numbers, skipping those that appear in the cumulative sum sequence.
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Runtime globals – built only once */
static bool _init_done = false;
static seq_t * _noncomps = NULL;   /* 1 followed by primes up to LIMIT */
static size_t _noncomp_cnt = 0;

/* Build the list of non‑composites: 1 and all primes ≤ LIMIT. */
static void _build_noncomps(void)
{
    if (_init_done) return;

    /* A generous limit – for n ≤ 100 000 this produces far more primes
       than we will ever need; it keeps the memory under a few megabytes. */
    const idx_t LIMIT = 200000;

    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= LIMIT; q += p)
                is_prime[q] = false;
        }
    }

    /* Count primes to size the array. */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;

    /* Non‑composites = 1 followed by the primes. */
    _noncomp_cnt = prime_cnt + 1;
    _noncomps = (seq_t*)malloc(_noncomp_cnt * sizeof(seq_t));
    if (!_noncomps) exit(1);

    _noncomps[0] = (seq_t)1;                /* the number 1 */
    size_t idx = 1;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            _noncomps[idx++] = (seq_t)i;    /* prime numbers */

    free(is_prime);
    _init_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, guard */

    _build_noncomps();             /* ensure non‑composites are ready */

    seq_t S_last = (seq_t)1;       /* current element of the S sequence */
    idx_t diff_idx = 0;            /* index of next non‑composite to add */
    idx_t comp_cnt = 0;            /* number of complements found */
    seq_t k = (seq_t)1;            /* candidate integer to test */

    while (comp_cnt < n) {
        /* Advance S until it is ≥ k. */
        while (S_last < k) {
            S_last += _noncomps[diff_idx];
            ++diff_idx;
        }

        if (S_last == k) {
            /* k is in the cumulative sum sequence – skip it. */
        } else {
            /* k is a complement. */
            ++comp_cnt;
            if (comp_cnt == n)
                return k;
        }
        ++k;   /* move to the next integer */
    }

    return (seq_t)0;   /* never reached */
}
