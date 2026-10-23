/*
 * A159044  Primes of the form prime(n)+prime(n+4)+1.
 * https://oeis.org/A159044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159044 (primes of the form prime(n)+prime(n+4)+1) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* ---------- sequence cache ---------------------------------------- */
static seq_t *seq = NULL;           /* array of found terms    */
static size_t seq_cap = 0;          /* capacity of seq         */
static size_t seq_len = 0;          /* number of terms stored  */

/* ---------- prime cache ------------------------------------------- */
static uint64_t *primes = NULL;     /* primes[1] = 2, primes[2] = 3, …   */
static size_t primes_cap = 0;       /* capacity of primes         */
static size_t primes_cnt = 0;       /* last used index (inclusive) */
static uint64_t current_candidate = 3; /* next odd candidate to test */
static bool primes_initialized = false;

/* ---------- other state --------------------------------------------- */
static size_t next_k = 2;          /* next k to test (1‑based)  */

/* ---------- local helpers ------------------------------------------- */
static void ensure_primes_upto_idx(size_t idx_needed)
{
    /* grow storage if required */
    if (idx_needed + 1 > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        while (new_cap < idx_needed + 1) new_cap *= 2;
        primes = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
        primes_cap = new_cap;
    }
    /* generate primes until we have defined indices up to idx_needed */
    while (primes_cnt <= idx_needed) {
        uint64_t p = current_candidate;
        bool isprime = true;
        for (size_t i = 2; i <= primes_cnt; ++i) {
            uint64_t sq = primes[i] * primes[i];
            if (sq > p) break;
            if (p % primes[i] == 0) { isprime = false; break; }
        }
        if (isprime) {
            primes_cnt++;
            primes[primes_cnt] = p;
        }
        current_candidate += 2;               /* skip even numbers */
    }
}

/* ---------- main routine ------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (seq == NULL) {
        seq_cap = 256;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    /* initialise prime list on first call */
    if (!primes_initialized) {
        primes_cap = 32;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes[1] = 2;          /* primes[1] == 2 */
        primes_cnt = 1;         /* stored primes: 1..primes_cnt */
        primes_initialized = true;
        current_candidate = 3;
    }

    /* generate terms until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* make sure we have primes up to k+4 */
        ensure_primes_upto_idx(next_k + 4);

        /* compute candidate = prime(k)+prime(k+4)+1 */
        unsigned __int128 sum = (unsigned __int128)primes[next_k]
                                + (unsigned __int128)primes[next_k + 4]
                                + 1;
        seq_t candidate = (seq_t)sum;   /* fits in seq_t for requested n */

        /* stash if it is prime */
        if (is_prime_big(candidate)) {
            if (seq_len >= seq_cap) {          /* grow cache */
                seq_cap <<= 1;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = candidate;
        }
        /* proceed to next k (regardless of primality) */
        next_k++;
    }

    return seq[(size_t)n - 1];
}
