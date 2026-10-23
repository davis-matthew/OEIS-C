/*
 * A106103  Primes with minimal digit = 3.
 * https://oeis.org/A106103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   is_prime_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106103 : primes whose minimal decimal digit is 3 */
/* The sequence starts 3, 37, 43, 53, 73, 83, 337, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_64.h"

static seq_t *seq_table = NULL;   /* stored primes */
static idx_t seq_size   = 0;      /* number of primes stored */
static idx_t seq_cap    = 0;      /* capacity of seq_table */
static uint64_t cand    = 3;      /* current candidate, always odd */

/* Ensure seq_table contains at least n primes satisfying the rule. */
static void ensure_size(idx_t n)
{
    while (seq_size < n) {
        /* Find minimal digit and presence of 3 in current candidate. */
        uint64_t tmp = cand;
        uint8_t min_digit = 9;
        bool has3 = false;
        while (tmp) {
            uint8_t d = tmp % 10;
            if (d < min_digit) min_digit = d;
            if (d == 3) has3 = true;
            if (min_digit < 3) break;    /* cannot satisfy condition */
            tmp /= 10;
        }

        if (min_digit == 3 && has3 && is_prime_64(cand)) {
            /* Store the prime. */
            if (seq_size >= seq_cap) {
                seq_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
                seq_table = (seq_t *)realloc(seq_table, seq_cap * sizeof(seq_t));
            }
            seq_table[seq_size++] = (seq_t)cand;
        }

        /* Advance to next odd candidate. */
        cand += 2;
    }
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but avoid crash */
    ensure_size(n);
    return seq_table[n - 1];
}
