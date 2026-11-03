/*
 * A056005  Number of 3-element ordered antichains on an unlabeled n-element
 *          set; T_1-hypergraphs with 3 labeled nodes and n hyperedges.
 * https://oeis.org/A056005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+7,7) - 6*C(n+5,5) + 6*C(n+4,4) + 3*C(n+3,3) -
 *            6*C(n+2,2) + 2*C(n+1,1) (Offset 0: a(0)=0, a(1)=0, a(2)=0)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 987 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t comb_small_int(int n, int k) {
    // Computes binomial coefficient C(n,k) for small k (k <= n <= 10007).
    unsigned __int128 num = 1;
    for (int i = 1; i <= k; ++i) {
        num = num * (unsigned __int128)(n - k + i);
        num /= (unsigned __int128)i;
    }
    return (seq_t)num;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;  /* a(0)=a(1)=a(2)=0 */

    /* Use integers large enough for intermediate calculations */
    seq_t c7 = comb_small_int((int)n + 7, 7);
    seq_t c5 = comb_small_int((int)n + 5, 5);
    seq_t c4 = comb_small_int((int)n + 4, 4);
    seq_t c3 = comb_small_int((int)n + 3, 3);
    seq_t c2 = comb_small_int((int)n + 2, 2);
    seq_t c1 = comb_small_int((int)n + 1, 1);

    seq_t result = c7;
    result -= (seq_t)6 * c5;
    result += (seq_t)6 * c4;
    result += (seq_t)3 * c3;
    result -= (seq_t)6 * c2;
    result += (seq_t)2 * c1;

    return result;
}
