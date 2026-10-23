/*
 * A131368  Coprime semiprimes: a(n-1) and a(n) are the closest coprime
 *          semiprimes.
 * https://oeis.org/A131368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131368 – Coprime semiprimes.
   a(1)=4.  Each successive term is the smallest semiprime larger than
   the previous term that is coprime to it.  The sequence is built
   incrementally; previously computed terms are cached in a static array.
   The implementation uses a sieve of primes (via build_primes_upto)
   for efficient semiprime detection. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/gcd.h"            /* seq_t gcd(seq_t, seq_t) */
#include "../utils/build_primes_upto.h" /* build_primes_upto() */
#include "../utils/int_sqrt.h"       /* int_sqrt(idx_t) */

/* ---------- static storage for the sequence ---------- */
static seq_t *seq_arr   = NULL;   /* array holding the terms          */
static size_t seq_cap   = 0;      /* allocated capacity of seq_arr    */
static size_t seq_len   = 0;      /* number of terms computed so far  */
static seq_t prev_val   = 0;      /* last term added */

/* ---------- static storage for the prime sieve ---------- */
static uint64_t *prime_list = NULL;/* list of primes up to prime_limit*/
static size_t    prime_cnt  = 0;   /* number of primes in prime_list  */
static idx_t     prime_limit = 0; /* maximum prime value currently in prime_list */

/* ----- ensure prime_list contains all primes <= limit ----- */
static void
ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free previous list if any */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* build a new prime list up to 'limit' */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* ----- test whether a 64‑bit integer is a semiprime ----- */
static bool
is_semiprime_uint64(uint64_t n)
{
    int cnt = 0;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp)
            break;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
            if (cnt > 2)
                return false;
        }
    }
    if (temp > 1)
        ++cnt;                       /* remaining prime factor */

    return cnt == 2;
}

/* ----- extend the sequence until it contains at least n terms ----- */
static void
extend_sequence(idx_t n)
{
    if (seq_arr == NULL) {           /* first initialisation */
        seq_cap = 32;
        seq_arr = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        ensure_primes_up_to(100);    /* small primes for first tests */
    }

    while (seq_len < n) {
        /* start searching from the number after the previous term */
        uint64_t start = (prev_val == 0) ? 4ULL : (uint64_t)prev_val + 1ULL;

        for (uint64_t cand = start; ; ++cand) {
            uint64_t sqrt_cand = (uint64_t)int_sqrt(cand) + 1ULL;
            ensure_primes_up_to(sqrt_cand);

            if (is_semiprime_uint64(cand) &&
                (prev_val == 0 || gcd(prev_val, (seq_t)cand) == 1)) {

                /* add cand to the sequence */
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                }
                seq_arr[seq_len++] = (seq_t)cand;
                prev_val          = (seq_t)cand;
                break;                /* next term found */
            }
        }
    }
}

/* ---------- public sequence function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0)          /* undefined, but return 0 for safety */
        return 0;

    extend_sequence(n);
    return seq_arr[n - 1];
}
