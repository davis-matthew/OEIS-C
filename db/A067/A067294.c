/*
 * A067294  Third column of triangle A028364.
 * https://oeis.org/A067294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((11*n^2 + 28*n + 15)/(2*(2*n+1)*(2*n+3))) * C(n+2) *
 *            where C(k) is the k‑th Catalan number
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/catalan.h"

/* Euclidean algorithm for seq_t – works for arbitrary‑precision types as
 * well as for built‑in unsigned integer types. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* numerator  = 11*n^2 + 28*n + 15   (seq_t arithmetic) */
    seq_t numerator = 11 * nseq * nseq + 28 * nseq + 15;

    /* denominator = 2*(2*n+1)*(2*n+3) */
    seq_t two_n_plus1 = 2 * nseq + 1;
    seq_t two_n_plus3 = 2 * nseq + 3;
    seq_t denominator = 2 * (two_n_plus1 * two_n_plus3);

    /* Catalan number C(n+2) */
    seq_t cat = catalan(n + (idx_t)2);

    /* Cancel common factors between cat and denominator */
    seq_t g = seq_gcd(cat, denominator);
    cat /= g;
    denominator /= g;

    /* Cancel common factors between numerator and remainder of denominator */
    g = seq_gcd(numerator, denominator);
    numerator /= g;
    denominator /= g;

    /* After cancellation the denominator should divide evenly. */
    seq_t result = cat * numerator;
    if (denominator != (seq_t)1) {
        result /= denominator;
    }
    return result;
}
