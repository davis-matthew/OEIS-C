/*
 * A085315  Numbers such that first reversing digits and after forming its
 *          cube equals the result of first-form-cube and after-reverse
 *          operation with exclusion of cases divisible by 10.
 * https://oeis.org/A085315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow_int_seq, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085315: numbers n such that rev(n³) = rev(n)³ and n is not divisible by 10.
    This implementation generates candidates by increasing decimal length,
    tests the reverse‑cube property for each, and returns the nth term (1‑based).
    The single–digit solutions 1, 2, 7 are handled explicitly; for length ≥2
    only numbers whose first and last digit are 1 and whose interior digits are
    0 or 1 are considered.  Candidates are produced in ascending numeric
    order, so the sequence is obtained directly without sorting.  */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/reverse_digits.h"   /* seq_t reverse_digits(seq_t x); */
#include "../utils/pow_int_seq.h"      /* seq_t pow_int_seq(seq_t base, idx_t exp); */

/* Return true iff n satisfies rev(n³)=rev(n)³. */
static bool good(seq_t n)
{
    seq_t rev_n   = reverse_digits(n);
    seq_t cube    = pow_int_seq(n, (idx_t)3);
    seq_t rev_cube= reverse_digits(cube);
    seq_t revn_cube= pow_int_seq(rev_n, (idx_t)3);
    return rev_cube == revn_cube;
}

/* External function required by the framework. */
seq_t a(idx_t N)
{
    idx_t count = 0;        /* number of valid terms found so far */

    /* Length 1: check the single‑digit numbers 1–9. */
    for (int d = 1; d <= 9; ++d) {
        seq_t cand = (seq_t)d;
        if (good(cand)) {
            ++count;
            if (count == N) return cand;
        }
    }

    /* Length ≥2: first and last digit 1, interior digits 0 or 1. */
    for (int len = 2; ; ++len) {
        size_t combos = (size_t)1 << (len - 2);   /* 2^(len‑2) middle patterns */
        for (size_t mask = 0; mask < combos; ++mask) {
            seq_t n = 1;            /* first digit */
            for (int bit = len - 3; bit >= 0; --bit) {
                int digit = (mask >> bit) & 1;
                n = n * (seq_t)10 + (seq_t)digit;
            }
            n = n * (seq_t)10 + (seq_t)1;  /* last digit */

            if (good(n)) {
                ++count;
                if (count == N) return n;
            }
        }
    }
}
