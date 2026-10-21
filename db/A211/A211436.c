/*
 * A211436  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and 2w+3x+4y=0.
 * https://oeis.org/A211436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A211436:
 * Number of ordered triples (w,x,y) with w,x,y∈[-n,…,n] and 2w+3x+4y=0.
 * The count is obtained by summing over all even x=2u and counting
 * admissible y satisfying |3u+2y| ≤ n.
 */
#include <stdint.h>

/* integer floor division for positive divisor */
static inline int64_t floor_div(int64_t a, int64_t b) {
    /* b > 0 */
    int64_t q = a / b;
    int64_t r = a % b;
    if (r && ((a < 0) != (b < 0))) q--;
    return q;
}

/* integer ceil division for positive divisor */
static inline int64_t ceil_div(int64_t a, int64_t b) {
    /* b > 0 */
    if (a >= 0) return (a + b - 1) / b;
    else return a / b;  /* trunc toward zero gives ceil for negative */
}

seq_t a(idx_t n) {
    seq_t count = 0;
    int64_t m = (int64_t)n;          /* signed copy of n */

    int64_t maxu = m / 2;            /* floor(n/2) */
    for (int64_t u = -maxu; u <= maxu; ++u) {
        /* lower = ceil( (-m - 3u) / 2 ) */
        int64_t a_val = -m - 3 * u;
        int64_t lower = ceil_div(a_val, 2);

        /* upper = floor( (m - 3u) / 2 ) */
        int64_t b_val = m - 3 * u;
        int64_t upper = floor_div(b_val, 2);

        /* clamp to y∈[-m,m] */
        if (lower < -m) lower = -m;
        if (upper >  m) upper =  m;

        if (upper >= lower) {
            count += (upper - lower + 1);
        }
    }
    return count;
}
