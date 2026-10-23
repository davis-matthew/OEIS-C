/*
 * A128556  a(1)=1. a(n) = the smallest positive multiple of d(a(n-1)) that
 *          does not occur earlier in the sequence, where d(m) is the number
 *          of positive divisors of m.
 * https://oeis.org/A128556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A128556
 *
 *  a(1) = 1
 *  a(n) = the smallest positive multiple of d(a(n-1))
 *  that has not yet appeared in the sequence.
 *  Here d(m) is the number of positive divisors of m.
 *
 *  The implementation caches computed terms and uses a naïve
 *  divisor‑count routine that works for arbitrary‑precision
 *  seq_t values as long as the standard arithmetic operators
 *  are defined for that type.  It is efficient enough for all
 *  tested values up to n ≈ 10 000 and uses only the standard
 *  C library plus the required C++ compatible headers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisor_count_seq_t.h"

/* ----------------------------------------------------------------------
 *  Local state: the generated part of the sequence.
 *  seq   – pointer to the array storing the terms
 *  seq_cap – capacity of the allocated array
 *  seq_len – number of terms that have already been computed
 * ---------------------------------------------------------------------- */
static seq_t *seq     = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* ----------------------------------------------------------------------
 *  Grow the cached array to hold at least 'need' elements.
 *  New capacity is a power of two not less than 'need'.
 * ---------------------------------------------------------------------- */
static void grow_seq(size_t need)
{
    if (need <= seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;

    seq_t *newp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq = newp;
    seq_cap = new_cap;
}

/* ----------------------------------------------------------------------
 *  Initialise the sequence with its first term.
 * ---------------------------------------------------------------------- */
static void init_seq(void)
{
    if (seq_len == 0) {
        grow_seq(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }
}

/* ----------------------------------------------------------------------
 *  Ensure that all terms up to index `n` (1‑based) have been computed.
 *  The routine proceeds sequentially and caches each new term.
 * ---------------------------------------------------------------------- */
static void ensure_up_to(idx_t n)
{
    if (n == 0) return;        /* undefined for 0, but harmless          */

    init_seq();

    if (seq_len >= (size_t)n) return;

    for (idx_t i = (idx_t)seq_len + 1; i <= n; ++i) {
        seq_t prev = seq[seq_len - 1];

        /* Compute d(a(n-1)) */
        seq_t d = divisor_count_seq_t(prev);

        /* Find the smallest multiple of d not seen before */
        for (seq_t mult = (seq_t)1;; ++mult) {
            seq_t candidate = d * mult;

            bool seen = false;
            for (size_t j = 0; j < seq_len; ++j) {
                if (seq[j] == candidate) {
                    seen = true;
                    break;
                }
            }

            if (!seen) {
                grow_seq(seq_len + 1);
                seq[seq_len++] = candidate;
                break;
            }
        }
    }
}

/* ----------------------------------------------------------------------
 *  Public function required by the problem statement.
 *  It may be called in any order and returns the n‑th element of
 *  the sequence (n ≥ 1).
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return seq[n - 1];
}
