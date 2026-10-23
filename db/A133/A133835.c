/*
 * A133835  Slowest increasing sequence of primes such that two neighbor terms
 *          share at least two digits (counted with multiplicity).
 * https://oeis.org/A133835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th term of the slowest increasing sequence of
 *            primes such that adjacent terms share at least two digits
 *            (counted with multiplicity). This implementation iteratively
 *            builds the sequence, caching all computed terms
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_frequencies, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build system. */
#include "../utils/is_prime_big.h"
#include "../utils/digit_frequencies.h"

/* Return true iff a and b share at least two digits (with multiplicity). */
static bool shares_two_digits(seq_t a, seq_t b)
{
    unsigned int fa[10] = {0};
    unsigned int fb[10] = {0};
    digit_frequencies(a, fa);
    digit_frequencies(b, fb);
    unsigned int overlap = 0;
    for (int d = 0; d < 10; ++d) {
        overlap += fa[d] < fb[d] ? fa[d] : fb[d];
    }
    return overlap >= 2;
}

/* Dynamic storage of the sequence. */
static seq_t *seq_buffer   = NULL;
static size_t seq_capacity = 0;
static size_t seq_length   = 0;

/* Grow the buffer enough to hold needed elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_capacity) return;
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
    while (new_cap < needed) new_cap *= 2U;
    seq_buffer = (seq_t *)realloc(seq_buffer, new_cap * sizeof(seq_t));
    seq_capacity = new_cap;
}

/* Main entry point.  n is 1‑based; a(1) = 11. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset < 1 is irrelevant */
    while (seq_length < (size_t)n) {
        seq_t next_prime;
        if (seq_length == 0) {
            /* first term of the sequence */
            next_prime = 11;
        } else {
            seq_t prev = seq_buffer[seq_length - 1];
            seq_t cand = prev + 1;
            /* keep only odd candidates (prev is odd). */
            if (cand % 2 == 0) cand += 1;
            /* search for the next prime that shares at least two digits
               with the previous prime. */
            while (true) {
                if (is_prime_big(cand) && shares_two_digits(prev, cand))
                    break;
                cand += 2;           /* stay odd */
            }
            next_prime = cand;
        }
        ensure_capacity(seq_length + 1);
        seq_buffer[seq_length++] = next_prime;
    }
    return seq_buffer[n - 1];
}
