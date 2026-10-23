/*
 * A035472  Coordination sequence for lattice D*_6 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 83, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(n) = (4*n*(31 + 10*n^2 + 4*n^4))/15 for n > 0 */

/* Computes the nth term of OEIS A035472 (coordination sequence for lattice D*_6). */
#include <stdint.h>

/* Euclidean algorithm for seq_t (works for built-in unsigned types and big‑int classes). */
static inline seq_t gcd_seq(seq_t a, seq_t b) {
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    seq_t nseq = n;
    seq_t n2 = nseq * nseq;          /* n^2 */
    seq_t n4 = n2 * n2;              /* n^4 */

    /* t = 31 + 10*n^2 + 4*n^4 */
    seq_t t = (seq_t)31 + (seq_t)10 * n2 + (seq_t)4 * n4;

    /* Compute (4*n*t) / 15 with cancellation to avoid overflow. */
    seq_t a_part = nseq;
    seq_t b_part = t;
    seq_t c_part = (seq_t)4;
    seq_t denom   = (seq_t)15;

    /* Cancel common factors */
    seq_t g = gcd_seq(a_part, denom);
    a_part /= g;
    denom   /= g;

    g = gcd_seq(b_part, denom);
    b_part /= g;
    denom   /= g;

    g = gcd_seq(c_part, denom);
    c_part /= g;
    denom   /= g;

    /* denom should now be 1; final multiplication. */
    seq_t prod = a_part;
    prod *= b_part;
    prod *= c_part;
    if (denom != (seq_t)1)
        prod /= denom;   /* safety, should never happen */

    return prod;
}
