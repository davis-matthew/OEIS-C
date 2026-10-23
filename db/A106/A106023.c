/*
 * A106023  Primes p such that 8*p +- 3, 28*p +- 3 and 38*p +- 3 are all
 *          primes.
 * https://oeis.org/A106023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Return the n-th prime p such that all of 8p±3, 28p±3, 38p±3 are
 *            also prime
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t next_candidate = 3;
static bool seq_initialized = false;

static bool passes(uint64_t p)
{
    /* 8p - 3 and 8p + 3 */
    uint64_t v;
    v = (uint64_t)((__int128)8 * p - 3);
    if (!is_prime_uint64(v)) return false;
    v = (uint64_t)((__int128)8 * p + 3);
    if (!is_prime_uint64(v)) return false;

    /* 28p - 3 and 28p + 3 */
    v = (uint64_t)((__int128)28 * p - 3);
    if (!is_prime_uint64(v)) return false;
    v = (uint64_t)((__int128)28 * p + 3);
    if (!is_prime_uint64(v)) return false;

    /* 38p - 3 and 38p + 3 */
    __int128 tmp = (__int128)38 * p + 3;
    if (tmp > UINT64_MAX) return false;          /* outside 64‑bit range */
    v = (uint64_t)tmp;
    if (!is_prime_uint64(v)) return false;

    tmp = (__int128)38 * p - 3;
    if (tmp > UINT64_MAX) return false;
    v = (uint64_t)tmp;
    return is_prime_uint64(v);
}

static void add_value(uint64_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = (seq_t)v;
}

void init_sequence(void)
{
    if (seq_initialized) return;
    add_value(2);                      /* 2 is always part of the sequence */
    next_candidate = 3;
    seq_initialized = true;
}

seq_t a(idx_t n)
{
    init_sequence();
    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;          /* skip even numbers */

        if (is_prime_uint64(cand) && passes(cand)) {
            add_value(cand);
        }
    }
    return seq[n - 1];
}
