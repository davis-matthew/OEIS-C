/*
 * A102498  Values of n for which the concatenation 3nn3 (from sequence
 *          A100896) are primes.
 * https://oeis.org/A102498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 0, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 965 (2026-11-03)
 * Helpers:   is_prime_seq, mem, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer n such that 3nn3 (concatenated in base 10) is a prime number. */
/* This implements OEIS A102498. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

seq_t a(idx_t n)
{
    static seq_t *values = NULL;
    static idx_t cap    = 0;
    static idx_t size   = 0;
    static idx_t next   = 1;          /* next candidate integer n */

    while (size <= n) {
        seq_t nval = (seq_t)next;
        int d = num_digits_idx(next);   /* number of decimal digits of n */

        seq_t part1 = pow10((idx_t)(2 * d + 1)) * (seq_t)3;
        seq_t part2 = pow10((idx_t)(d + 1)) * nval;
        seq_t part3 = nval * (seq_t)10;
        seq_t num   = part1 + part2 + part3 + (seq_t)3;

        if (is_prime_seq(num))
            push_seq(&values, &cap, &size, nval);

        ++next;
    }
    return values[n];
}
