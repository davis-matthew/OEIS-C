/*
 * A057881  Primes with 5 distinct digits that remain prime (no leading zeros
 *          allowed) after deleting all occurrences of its digits d.
 * https://oeis.org/A057881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   good_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A057881: primes with 5 distinct digits that remain prime after
 * deleting all occurrences of each of its digits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/good_prime.h"

/* ---------- main sequence logic ---------- */
static seq_t *seq_vals   = NULL;   /* cached sequence values */
static size_t  seq_cap   = 0;      /* allocated capacity */
static size_t  seq_len   = 0;      /* number of values stored */

/* Ensure we have at least n terms computed. */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;      /* already have enough */

    /* Build fresh sequence from scratch, increasing sieve limit as needed. */
    idx_t limit = 100000;                   /* initial bound */

    while (1) {
        /* Prime sieve up to 'limit' */
        uint8_t *prime_flag = (uint8_t *)malloc((size_t)(limit + 1));
        if (!prime_flag) return;             /* memory failure */
        memset(prime_flag, 1, (size_t)(limit + 1));
        prime_flag[0] = prime_flag[1] = 0;

        for (idx_t i = 2; i * i <= limit; ++i) {
            if (prime_flag[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    prime_flag[j] = 0;
            }
        }

        /* Reset sequence storage for this run */
        seq_len = 0;
        seq_cap = 0;

        /* Scan all primes up to limit and collect qualifying ones */
        for (idx_t val = 2; val <= limit; ++val) {
            if (!prime_flag[val]) continue;
            if (good_prime((uint32_t)val, prime_flag, limit)) {
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
                    if (!tmp) { free(prime_flag); return; }
                    seq_vals = tmp;
                    seq_cap = new_cap;
                }
                seq_vals[seq_len++] = (seq_t)val;
                if (seq_len >= (size_t)n) break;
            }
        }

        free(prime_flag);
        if (seq_len >= (size_t)n) break;    /* enough terms collected */
        limit *= 2;                         /* increase bound and try again */
    }
}

/* Public API required by the test harness */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq_vals[n - 1];
}
