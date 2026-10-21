/*
 * A232854  Number of (n+1)X(1+1) 0..1 arrays x(i,j) with row sums
 *          sum{j^3*x(i,j), j=1..1+1} nondecreasing, and column sums
 *          sum{i^3*x(i,j), i=1..n+1} nondecreasing.
 * https://oeis.org/A232854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nondecreasing quadruples (c0,c1,c8,c9) of
 *            nonnegative integers summing to n+1, where 2·P(c0+c1) – P(c0) ≤
 *            P(c0+c1+c8), with P(k)=∑_{i=1}^k i^3. Implemented by counting
 *            all triples (a,b,c) with 0≤a≤b≤c≤m (m=n+1) satisfying the
 *            inequality 2·P(b)-P(a) ≤ P(c). See OEIS A232854 for de...
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t m = n + 1;                       // number of rows
    seq_t *P = (seq_t*)malloc((m + 1) * sizeof(seq_t));
    if (!P) return (seq_t)0;
    P[0] = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t cube = (seq_t)k * k * k;     // k^3
        P[k] = P[k - 1] + cube;           // prefix sum of cubes
    }

    seq_t count = 0;
    for (idx_t a = 0; a <= m; ++a) {
        idx_t c_ptr = a;                  // minimal c for current a,b
        for (idx_t b = a; b <= m; ++b) {
            seq_t T = 2 * P[b] - P[a];
            if (c_ptr < b) c_ptr = b;    // ensure c ≥ b
            while (c_ptr <= m && P[c_ptr] < T) {
                ++c_ptr;                  // find minimal c satisfying inequality
            }
            if (c_ptr > m) break;        // no valid c for this or larger b
            count += (seq_t)(m - c_ptr + 1);  // all c ∈ [c_ptr, m] are valid
        }
    }

    free(P);
    return count;
}
