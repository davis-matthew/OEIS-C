/*
 * A005332  Certain subgraphs of a directed graph.
 * https://oeis.org/A005332
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-2} C(n-1,i) * p(n-1-i) * 2^i *
 *            Sum_{j=0..n-2-i} (-1)^j*(n-1-i-j)/p(j), p(n) = Product_{k=1..n}
 *            (2^k-1). Implemented as a(n) = Sum_{m=1..n-1} C(n-1,m) *
 *            2^(n-1-m) * S(m), where S(m) = Sum_{j=0..m}
 *            (-1)^j*(m-j)*p(m)/p(j) and U(m) = Sum_{j=0..m} (-1)^j*p(m)/p(j)
 *            satisfy S(m) = (2^...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t shl_seq(seq_t x, idx_t e) {
    if (seq_is_fixed_width() && e >= (idx_t)(8 * sizeof(seq_t)))
        return (seq_t)0;
    return (seq_t)(x << e);
}

/* x * (2^m - 1) */
static seq_t mul_mersenne(seq_t x, idx_t m) {
    return (seq_t)(shl_seq(x, m) - x);
}

seq_t a(idx_t n) {
    seq_t S = 0, U = 1, result = 0;
    for (idx_t m = 1; m + 1 <= n; m++) {
        seq_t nS = mul_mersenne((seq_t)(S + U), m);
        seq_t nU = mul_mersenne(U, m);
        if (m % 2 == 0)
            nU = (seq_t)(nU + 1);
        else
            nU = (seq_t)(nU - 1);
        S = nS;
        U = nU;
        seq_t term = (seq_t)(binom(n - 1, m) * S);
        result = (seq_t)(result + shl_seq(term, n - 1 - m));
    }
    return result;
}
