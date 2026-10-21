/*
 * A078204  Smallest multiple of the concatenation of 1 up to n that begins
 *          with the concatenation of n down to 1.
 * https://oeis.org/A078204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A078204: the smallest multiple of concat(1…n)
   that begins with concat(n…1).  For each n ≥ 1 the function returns
   the value a(n).  The implementation iteratively searches for the
   smallest power of 10, 10^k, such that the remainder
   r = (-concat(n…1) * 10^k) mod concat(1…n) satisfies r < 10^k. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* Build L = concat(1..n) */
    seq_t L = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t d = (idx_t)digits(i);
        seq_t mult = pow10(d);
        L = L * mult + (seq_t)i;
    }

    /* Build R = concat(n..1) */
    seq_t R = 0;
    for (idx_t j = n; j > 0; --j) {
        idx_t d = (idx_t)digits(j);
        seq_t mult = pow10(d);
        R = R * mult + (seq_t)j;
    }

    /* Compute r = (-R) mod L  (i.e., the smallest non‑negative remainder) */
    seq_t rem = R % L;
    seq_t r = (L - rem) % L;

    /* pow10k = 10^k, starting with 10^0 = 1 */
    seq_t pow10k = 1;

    while (1) {
        /* If r fits into the current 10^k, we have found the smallest k. */
        if (r < pow10k)
            return R * pow10k + r;

        /* Advance to the next k: r_{k+1} = r_k * 10 (mod L) */
        r = (r * 10) % L;
        pow10k *= 10;
    }
}
