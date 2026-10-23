/*
 * A184258  Number of strings of numbers x(i=1..3) in 0..n with sum i^3*x(i)
 *          equal to 27*n.
 * https://oeis.org/A184258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x1,x2,x3) with 0 ≤ x_i ≤ n such that
 *            x1 + 8*x2 + 27*x3 = 27*n (n ≥ 1)
 * Range:     u8 n <= 9, u16 n <= 998, u32 n <= 998, u64 n <= 998,
 *            u128 n <= 998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1; a(0) unused */

    seq_t N = (seq_t)n;                     /* cast index to seq_t for arithmetic */
    seq_t total = 0;

    for (idx_t x3 = 0; x3 <= n; ++x3) {
        seq_t t1 = N - (seq_t)x3;          /* N - x3   */
        seq_t rhs = (seq_t)27 * t1;        /* 27 * (N - x3) */

        seq_t upper = rhs / 8;             /* floor(27*(N-x3)/8) */
        if (upper > N) upper = N;          /* x2 ≤ N */

        seq_t lower = 0;
        if (rhs > N) {                     /* ceil((rhs - N)/8) */
            seq_t numer = rhs - N;         /* rhs - N ≥ 0 */
            lower = numer / 8 + ((numer % 8) ? 1 : 0);
        }

        if (upper >= lower)
            total += (upper - lower + 1);
    }

    return total;
}
