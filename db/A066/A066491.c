/*
 * A066491  Twin prime gaps: Pairs k, k+1 such that f(k) = f(k+1), where f is
 *          the prime gaps function given by f(m) = prime(m+1)-prime(m) and
 *          prime(m) denotes the m-th prime.
 * https://oeis.org/A066491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A066491 – Twin prime gaps.  
   a(n) is the (n‑th) term of the sorted list of indices k where  
   the gap between prime(k) and prime(k+1) equals the gap between  
   prime(k+1) and prime(k+2).  The function builds primes iteratively  
   and records every pair (k, k+1).  Complexity is O(N log N) for the  
   first N terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state used to memoise primes and sequence values. */
static idx_t *primes = NULL;
static size_t primes_cap = 0, primes_cnt = 0;

static seq_t *seq = NULL;
static size_t seq_cap = 0, seq_cnt = 0;

static idx_t current_candidate = 0;      /* next odd number to test */
static idx_t last_gap = 0;              /* gap between last two primes */

/* Ensure the primes array is large enough. */
static void ensure_prime_cap(size_t needed) {
    if (primes_cap < needed) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Ensure the sequence array is large enough. */
static void ensure_seq_cap(size_t needed) {
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Test whether n is prime using the known primes up to sqrt(n). */
static bool is_prime_candidate(idx_t n) {
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if ((unsigned __int128)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Initialise static data once. */
static void init_static(void) {
    if (primes_cnt > 0) return;   /* already initialised */

    ensure_prime_cap(10);
    primes[0] = 2;
    primes[1] = 3;
    primes_cnt = 2;
    last_gap = primes[1] - primes[0];   /* 1 */
    current_candidate = 5;              /* next odd to test */

    ensure_seq_cap(20);
    seq_cnt = 0;
}

/* Main function defining the sequence. */
seq_t a(idx_t n) {
    init_static();

    while (seq_cnt < n) {
        /* Find the next prime. */
        idx_t cand = current_candidate;
        while (!is_prime_candidate(cand)) {
            cand += 2;  /* skip evens */
        }

        /* Compute gap between new prime and the previous prime. */
        idx_t new_gap = cand - primes[primes_cnt - 1];

        /* If the new gap equals the previous gap, record the pair of indices. */
        if (last_gap != 0 && new_gap == last_gap) {
            /* The equal gaps involve primes at indices (primes_cnt-1) and primes_cnt
               in 1‑based terms. */
            ensure_seq_cap(seq_cnt + 2);
            seq[seq_cnt++] = (seq_t)(primes_cnt - 1);
            seq[seq_cnt++] = (seq_t)primes_cnt;
        }

        /* Append the new prime and update state. */
        ensure_prime_cap(primes_cnt + 1);
        primes[primes_cnt++] = cand;
        last_gap = new_gap;
        current_candidate = cand + 2;
    }

    return seq[n - 1];
}
