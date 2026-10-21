/*
 * A097938  Primes of the form prime(n)^2 - prime(n+1) - 1.
 * https://oeis.org/A097938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_idx_arr, grow_seq_arr, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A097938
    a(n) = prime(n)^2 − prime(n+1) − 1      (prime(n) is the n‑th prime)
    The function returns the n‑th term of the sequence of primes that
    satisfy this expression.  It works for arbitrary‑precision seq_t
    (via casting) and relies on the prime test in ../utils/isPrime.h.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrime.h"
#include "../utils/grow_idx_arr.h"
#include "../utils/grow_seq_arr.h"

/* ---------- prime table ---------- */
static idx_t   *prime_arr    = NULL;
static size_t   prime_len    = 0;
static size_t   prime_cap    = 0;

/* initialize the prime list with 2, 3 */
static void init_prime_arr(void) {
    if (prime_cap == 0) {
        grow_idx_arr(&prime_arr, &prime_len, &prime_cap);
        prime_arr[prime_len++] = 2;   /* prime(1) */
        prime_arr[prime_len++] = 3;   /* prime(2) */
    }
}

/* return the next prime after the last stored one */
static idx_t next_prime(void) {
    init_prime_arr();          /* ensure we have the first two primes */

    idx_t cand = prime_arr[prime_len - 1] + 1;
    if ((cand & 1) == 0) cand++;      /* skip even numbers */

    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            idx_t p = prime_arr[i];
            if ((uint64_t)p * (uint64_t)p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) break;
        cand += 2;                     /* next odd candidate */
    }

    grow_idx_arr(&prime_arr, &prime_len, &prime_cap);
    prime_arr[prime_len++] = cand;
    return cand;
}

/* ---------- sequence storage ---------- */
static seq_t  *term_arr   = NULL;
static size_t  term_len   = 0;
static size_t  term_cap   = 0;

/* index of the prime pair to examine next */
static idx_t  prime_scan = 0;

/* --------------------------------------------------------------- */
/*  External function required by the test harness                 */
/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but safe return */

    /* generate terms until we have the nth one */
    while (term_len < n) {
        /* ensure we have primes for the current scan index */
        while (prime_len <= prime_scan + 1)
            next_prime();

        uint64_t p_n   = (uint64_t)prime_arr[prime_scan];
        uint64_t p_np1 = (uint64_t)prime_arr[prime_scan + 1];

        /* compute candidate value */
        uint64_t cand = p_n * p_n - p_np1 - 1ULL;

        /* accept only if cand is prime */
        if (cand >= 2 && isPrime((idx_t)cand)) {
            grow_seq_arr(&term_arr, &term_len, &term_cap);
            term_arr[term_len++] = (seq_t)cand;
        }

        /* move to the next pair of primes */
        ++prime_scan;
    }

    return term_arr[n - 1];   /* sequence is 1‑based */
}
