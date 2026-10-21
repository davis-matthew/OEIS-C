/*
 * A239435  Values of k for which the equation x^2 - 25*y^2 = k has integer
 *          solutions.
 * https://oeis.org/A239435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239435
   a(n) is the n-th nonnegative integer k such that the Pellian
   equation x^2 – 25 y^2 = k has integer solutions.
   This is equivalent to the existence of non‑negative integers
   a,b with a·b = k and a≡b (mod 10). */

/* The code finds a(n) by enumerating successive integers k and,
   for each k, testing whether it has a divisor d with d ≡ k/d (mod 10). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static bool has_same_last_digit_factor(seq_t k)
{
    /* k >= 1 */
    idx_t sq = (idx_t)floor_sqrt(k);
    for (idx_t d = 1; d <= sq; ++d) {
        if (k % (seq_t)d == 0) {
            seq_t t = k / (seq_t)d;
            if ((seq_t)d % 10 == t % 10)
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;    /* offset 1: a(1) = 0 */

    idx_t count = 1;        /* 0 already counted */
    seq_t k = 1;
    for (;;) {
        if (has_same_last_digit_factor(k)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
