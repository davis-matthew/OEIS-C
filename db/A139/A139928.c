/*
 * A139928  Primes of the form 2x^2+165y^2.
 * https://oeis.org/A139928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 2x^2 + 165y^2, offset 1 (a(1)=2)
 * Range:     u8 n <= 4, u16 n <= 413, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"

/* Cached sequence values */
static seq_t *seq_terms = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Residue table for primes of the desired form */
static bool residue[1320] = {0};
static bool residue_initialized = false;

static inline void init_residue(void)
{
    if (residue_initialized)
        return;
    const int remainders[] = {
        2,167,173,197,263,293,413,437,503,527,557,
        623,677,743,767,887,893,1007,1157,1223,1253
    };
    const size_t cnt = sizeof(remainders)/sizeof(remainders[0]);
    for (size_t i = 0; i < cnt; ++i)
        residue[remainders[i]] = true;
    residue_initialized = true;
}

static void append_seq(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? (seq_cap << 1) : 16;
        seq_terms = (seq_t*)realloc(seq_terms, seq_cap * sizeof(seq_t));
    }
    seq_terms[seq_len++] = v;
}

static void ensure_terms(idx_t n)
{
    init_residue();

    static bool first_done = false;
    static uint64_t candidate = 3;

    if (!first_done) {
        append_seq((seq_t)2u);   /* a(1) = 2 */
        first_done = true;
    }

    while (seq_len < n) {
        if (is_prime(candidate) && residue[(unsigned)(candidate % 1320u)]) {
            append_seq((seq_t)candidate);
        }
        candidate += 2;   /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    ensure_terms(n);
    return seq_terms[n - 1];
}
