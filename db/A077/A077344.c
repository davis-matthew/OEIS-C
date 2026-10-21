/*
 * A077344  Triangle in which n-th row contains n smallest primes beginning
 *          with n.
 * https://oeis.org/A077344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1221, u32 n <= 1537, u64 n <= 9602,
 *            u128 n <= 9602, big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077344:
   a(n) is the nth element of the triangle whose r‑th row contains the
   first r primes whose decimal representation begins with the digits
   of r.  The function finds the row containing n and then scans the
   appropriate range of numbers, checking for primality with is_prime_big.
   The search is efficient because r ≤ O(√n) (≈141 for n≤10000),
   so only a few thousand candidates are tested for each call. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but return 0 for safety */

    /* ---- find the row r such that r*(r-1)/2 < n <= r*(r+1)/2 ---- */
    idx_t r = 1;
    while (true) {
        seq_t tri = (seq_t)r * ((seq_t)r + 1) / 2;
        if (tri >= (seq_t)n)
            break;
        ++r;
    }

    /* position of n within this row (1‑based) */
    seq_t prev = (seq_t)(r - 1) * ((seq_t)r) / 2;
    idx_t k = (idx_t)((seq_t)n - prev);

    /* ---- find the k‑th prime whose decimal representation starts with r ---- */
    seq_t prefix = (seq_t)r;
    idx_t found = 0;

    for (idx_t suffix = 0; ; ++suffix) {
        /* multiplier = 10^suffix */
        seq_t mult = pow10(suffix);
        seq_t start   = prefix * mult;
        seq_t end     = (prefix + 1) * mult - 1;

        for (seq_t cand = start; cand <= end; ++cand) {
            if (is_prime_big(cand)) {
                ++found;
                if (found == k)
                    return cand;
            }
        }
        /* if we reach here, continue with the next suffix length */
    }
}
