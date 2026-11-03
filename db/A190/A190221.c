/*
 * A190221  Numbers all of whose divisors are numbers whose decimal digits are
 *          in nondecreasing order.
 * https://oeis.org/A190221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 570, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 80 terms through n = 971 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A190221 by brute‑force search for numbers whose every divisor has nondecreasing decimal digits. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether decimal digits of x are nondecreasing (from most to least significant). */
static bool nondec_digits(uint64_t x)
{
    const uint64_t TEN = 10;
    int prev = 10;                     /* greater than any digit */
    while (x > 0) {
        int digit = (int)(x % TEN);
        if (digit > prev) return false;
        prev = digit;
        x /= TEN;
    }
    return true;
}

/* Check whether all divisors of m have nondecreasing decimal digits. */
static bool satisfies(uint64_t m)
{
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            if (!nondec_digits(d))
                return false;
            uint64_t e = m / d;
            if (e != d && !nondec_digits(e))
                return false;
        }
    }
    return true;
}

/* Static storage for the computed sequence. */
static size_t seq_len = 0;          /* number of terms stored */
static size_t seq_cap = 0;          /* capacity of seq array */
static seq_t *seq = NULL;           /* array of terms */
static uint64_t next_candidate = 1; /* integer to test next */

/* Return the n-th term of A190221 (a(1)=first term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;       /* undefined for offset < 1 */

    /* Grow the sequence until we have at least n terms. */
    while (seq_len < (size_t)n) {
        if (satisfies(next_candidate)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, seq, seq_cap);
                if (!tmp) {          /* allocation failed; abort */
                    mem_free(seq);
                    seq = NULL;
                    seq_cap = 0;
                    seq_len = 0;
                    return (seq_t)0;
                }
                seq = tmp;
            }
            seq[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq[n - 1];
}
