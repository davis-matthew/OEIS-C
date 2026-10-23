/*
 * A197861  Prime divisor of n which appears the fewest times previously in
 *          the sequence, with ties to the smaller prime.
 * https://oeis.org/A197861
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the prime divisor of n that has appeared the fewest
 * times among a(2)...a(n‑1); ties are broken by choosing the smaller prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/factor_u64.h"

typedef struct {
    uint64_t prime;
    int count;
} prime_entry;

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static idx_t computed_upto = 1;         /* last computed index */

static prime_entry *p_entries = NULL;
static size_t p_cap = 0, p_len = 0;

static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cap = new_cap;
}

static void ensure_p_capacity(size_t need)
{
    if (need <= p_cap) return;
    size_t new_cap = p_cap ? p_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    prime_entry *tmp = (prime_entry*)realloc(p_entries, new_cap * sizeof(prime_entry));
    p_entries = tmp;
    p_cap = new_cap;
}

static int find_p_index(uint64_t p)
{
    for (size_t i = 0; i < p_len; ++i)
        if (p_entries[i].prime == p) return (int)i;
    return -1;
}

static int get_p_count(uint64_t p)
{
    int idx = find_p_index(p);
    return (idx >= 0) ? p_entries[idx].count : 0;
}

static void increment_p_count(uint64_t p)
{
    int idx = find_p_index(p);
    if (idx >= 0) {
        p_entries[idx].count++;
    } else {
        ensure_p_capacity(p_len + 1);
        p_entries[p_len].prime = p;
        p_entries[p_len].count = 1;
        p_len++;
    }
}

seq_t a(idx_t n)
{
    if (n <= computed_upto) {
        return seq_cache[n];
    }

    /* extend cache up to n */
    ensure_seq_capacity(n + 1);
    for (idx_t k = computed_upto + 1; k <= n; ++k) {
        uint64_t primes_buf[64];
        uint64_t exps_buf[64];
        size_t pf_cnt = 0;
        factor_u64((uint64_t)k, primes_buf, exps_buf, &pf_cnt);

        uint64_t chosen = 0;
        int best_count = INT_MAX;
        for (size_t i = 0; i < pf_cnt; ++i) {
            uint64_t p = primes_buf[i];
            int cnt = get_p_count(p);
            if (cnt < best_count || (cnt == best_count && p < chosen)) {
                best_count = cnt;
                chosen = p;
            }
        }
        seq_cache[k] = (seq_t)chosen;
        increment_p_count(chosen);
    }
    computed_upto = n;
    return seq_cache[n];
}
