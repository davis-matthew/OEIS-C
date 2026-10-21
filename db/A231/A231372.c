/*
 * A231372  Squarefree composite numbers k such that 10 is a primitive root
 *          for all prime factors of k.
 * https://oeis.org/A231372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1711, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the squarefree composite numbers k for which 10 is a primitive
   root modulo every prime factor of k.  The sequence is generated
   progressively; the function expands its internal table as needed to
   answer any index n ≥ 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mod_pow.h"

/* Forward declaration of a helper that rebuilds the table up to a
   specified limit. */
static void build_table_upto(size_t limit,
                             seq_t **array_out,
                             size_t *len_out,
                             size_t *cap_out)
{
    /* Allocate temporary arrays for the sieve.  All allocations use
       casts to satisfy C++ requirements. */
    uint32_t *spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint8_t *full_flag = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));

    /* Prime storage for factorisation. */
    size_t primes_cap = 300000;
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_cnt = 0;

    /* Sieve of Eratosthenes : fill spf and prime list. */
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {                      /* i is prime */
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;

            if ((uint64_t)i * (uint64_t)i <= limit) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                    if (!spf[(size_t)j]) spf[(size_t)j] = (uint32_t)i;
                }
            }
        }
    }

    /* Determine which primes are full reptend (10 is primitive root). */
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint32_t p = primes[idx];
        if (p == 2 || p == 5) {
            full_flag[p] = 0;               /* exclude trivial cases */
            continue;
        }

        uint64_t m     = (uint64_t)p - 1;
        uint64_t tmp   = m;
        bool ok = true;

        for (size_t j = 0; j < primes_cnt &&
                            (uint64_t)primes[j] * (uint64_t)primes[j] <= tmp;
             ++j) {
            uint32_t q = primes[j];
            if (tmp % q == 0) {
                seq_t t = mod_pow((seq_t)10, m / q, (seq_t)p);
                if (t == (seq_t)1) { ok = false; break; }
                while (tmp % q == 0) tmp /= q;
            }
        }
        if (ok && tmp > 1) {
            seq_t t = mod_pow((seq_t)10, m / tmp, (seq_t)p);
            if (t == (seq_t)1) ok = false;
        }
        full_flag[p] = ok ? 1 : 0;
    }

    /* The prime array is no longer needed. */
    free(primes);

    /* Build the sequence array. */
    size_t seq_cap = 1024;
    seq_t *seq_array = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    size_t seq_len = 0;

    for (size_t x = 2; x <= limit; ++x) {
        if (spf[(size_t)x] == (uint32_t)x) continue;   /* skip primes */

        uint64_t tmp = x;
        bool squarefree = true;
        bool full = true;

        while (tmp > 1) {
            uint32_t pf = spf[(size_t)tmp];
            if (!full_flag[pf]) { full = false; break; }

            uint32_t cnt = 0;
            while (tmp % pf == 0) {
                tmp /= pf;
                ++cnt;
            }
            if (cnt > 1) { squarefree = false; break; }
        }

        if (!squarefree || !full) continue;

        /* Store the found number. */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq_array = (seq_t*)realloc(seq_array, seq_cap * sizeof(seq_t));
        }
        seq_array[seq_len++] = (seq_t)x;
    }

    /* Free temporary data. */
    free(spf);
    free(full_flag);

    /* Return results to the caller. */
    *array_out = seq_array;
    *len_out = seq_len;
    *cap_out = seq_cap;
}

/* Global state used to remember the current table of sequence values. */
static seq_t *global_seq_array = NULL;
static size_t global_seq_len   = 0;
static size_t global_seq_cap   = 0;
static size_t global_limit     = 1000000;   /* initial sieve limit */

/* Ensure that global_seq_array contains at least n terms.  The table
   is rebuilt from scratch as the limit grows. */
static void ensure_terms(idx_t n)
{
    if (global_seq_len >= (size_t)n) return;   /* already enough */

    /* Release any old table. */
    if (global_seq_array) free(global_seq_array);
    global_seq_array = NULL;
    global_seq_len   = 0;
    global_seq_cap   = 0;

    /* Grow the limit until enough terms are produced. */
    while (global_seq_len < (size_t)n) {
        build_table_upto(global_limit,
                         &global_seq_array,
                         &global_seq_len,
                         &global_seq_cap);
        if (global_seq_len >= (size_t)n) break;
        if (global_limit > SIZE_MAX / 2) break; /* safety against overflow */
        global_limit <<= 1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* undefined for n < 1 */

    ensure_terms(n);

    /* If we still do not have enough terms, return 0 as safe fallback. */
    if ((size_t)n > global_seq_len) return (seq_t)0;

    return global_seq_array[(size_t)n - 1];
}
