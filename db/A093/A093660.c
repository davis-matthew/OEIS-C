/*
 * A093660  Row sums of lower triangular matrix A093658.
 * https://oeis.org/A093660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2^n) = floor(n!·e) (preferential arrangements), a(2^n+2^m) =
 *            a(2^(m+1)) for n>m≥0. This code computes a(n) by repeatedly
 *            merging the two smallest exponents of the binary representation
 *            of n, the remaining exponent k giving a(n) = floor(k!·e)
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t S[65];
static int initialized = 0;

seq_t a(idx_t n)
{
    /* initialise floor(k!·e) table up to k=64 */
    if (!initialized) {
        initialized = 1;
        S[0] = (seq_t)1;                     /* floor(0!·e) = 1 */
        for (idx_t k = 1; k < 65; ++k)
            S[(size_t)k] = (seq_t)k * S[(size_t)(k - 1)] + (seq_t)1;
    }

    /* collect exponents of set bits of n */
    idx_t exps[65];
    idx_t count = 0;
    idx_t pos   = 0;
    idx_t tmp   = n;
    while (tmp) {
        if (tmp & (idx_t)1)
            exps[count++] = pos;
        tmp >>= 1;
        ++pos;
    }

    /* if n is a power of two: return directly */
    if (count == 1) {
        idx_t k = exps[0];
        return S[(size_t)k];
    }

    /* merge the two smallest exponents until one remains */
    while (count > 1) {
        /* find indices of the two smallest exponents */
        idx_t a = 0, b = 1;
        if (exps[a] > exps[b]) { idx_t t = a; a = b; b = t; }

        for (idx_t i = 2; i < count; ++i) {
            if (exps[i] < exps[a]) {
                b = a;
                a = i;
            } else if (exps[i] < exps[b]) {
                b = i;
            }
        }

        /* merge: replace smallest by exponent+1, discard the other */
        exps[a] = exps[a] + (idx_t)1;
        exps[b] = exps[count - 1];
        --count;
    }

    /* final exponent gives the answer */
    idx_t k = exps[0];
    return S[(size_t)k];
}
