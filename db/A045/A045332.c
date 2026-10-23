/*
 * A045332  Primes congruent to {0, 1, 2, 3} (mod 7).
 * https://oeis.org/A045332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 0,1,2,3 (mod 7)
   Implementation: iterate increasing integers, test primality,
   and keep those whose residue modulo 7 is 0,1,2 or 3.
   Results are memoised so that repeated calls (in any order)
   are fast.  The algorithm is O(n log n) and easily handles
   n up to at least 10 000 within the required time.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utility functions provided by the framework */
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

/* Memoised sequence data */
static seq_t *seq = NULL;          /* array of terms */
static size_t  seq_cap = 0;        /* allocated capacity */
static size_t  seq_len = 0;        /* number of terms stored */

/* Scan progress */
static idx_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure that seq has at least n terms stored. */
static void ensure_seq(idx_t n)
{
    if (seq_len >= n)
        return;                   /* already have enough terms */

    /* Initial allocation if needed */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < n) {
        /* Test current candidate for primality */
        if (is_prime((idx_t)next_candidate)) {
            uint64_t r = next_candidate % 7ull;   /* residue modulo 7 */
            if (r <= 3ull) {                      /* 0,1,2,3 */
                /* Grow array if necessary */
                if (seq_len == seq_cap)
                    grow_seq(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;   /* advance to next integer */
    }
}

/* Public interface required by the build system */
seq_t a(idx_t n)
{
    /* n is 1‑based by OEIS convention; behaviour for n < 1 is undefined */
    ensure_seq(n);
    return seq[n - 1];
}
