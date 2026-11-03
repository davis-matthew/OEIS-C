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
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A057881: primes with 5 distinct digits that remain prime after
 * deleting all occurrences of each of its digits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* ---------- helpers ---------- */
static bool good_prime(uint32_t p, const uint8_t *prime_flag, idx_t max_limit)
{
    /* Extract digits (reversed) */
    int digs[12], len = 0;
    uint32_t t = p;
    while (t) {
        digs[len++] = t % 10;
        t /= 10;
    }

    /* Build digit mask and count distinct digits */
    uint16_t mask = 0;
    for (int i = 0; i < len; ++i) mask |= 1u << digs[i];
    if (__builtin_popcount(mask) != 5) return false;

    /* Test each distinct digit */
    for (int d = 0; d < 10; ++d) {
        if ((mask & (1u << d)) == 0) continue;

        uint32_t new_num = 0;
        bool first = false, bad = false;
        for (int i = len - 1; i >= 0; --i) {
            int dig = digs[i];
            if (dig == d) continue;
            if (!first) {
                if (dig == 0) { bad = true; break; }
                first = true;
            }
            new_num = new_num * 10 + dig;
        }
        if (bad || new_num < 2 || new_num > max_limit || !prime_flag[new_num])
            return false;
    }
    return true;
}

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
                    seq_t *tmp = mem_resize(seq_t, seq_vals, new_cap);
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
