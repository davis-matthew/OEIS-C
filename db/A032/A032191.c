/*
 * A032191  Number of necklaces with 6 black beads and n-6 white beads.
 * https://oeis.org/A032191
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{d|gcd(n,6)} φ(d) * C(n/d, 6/d)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* φ(1)=1, φ(2)=1, φ(3)=2, φ(6)=2 */

#include <stddef.h>

/* Compute binomial coefficient C(N,K) for small K (<=6) */
static inline seq_t binom(idx_t N, idx_t K) {
    if (K > N) return (seq_t)0;
    if (K > N - K) K = N - K;          /* use the smaller side */
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i) / (seq_t)i;   /* exact division */
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;   /* behaviour below the offset not required */

    seq_t sum = 0;
    /* divisor 1 */
    sum += binom(n, 6);                      /* φ(1)=1 */

    /* divisor 2 */
    if ((n % 2) == 0) {
        sum += binom(n / 2, 3);              /* φ(2)=1 */
    }

    /* divisor 3 */
    if ((n % 3) == 0) {
        sum += 2 * binom(n / 3, 2);          /* φ(3)=2 */
    }

    /* divisor 6 */
    if ((n % 6) == 0) {
        /* C(n/6,1) = n/6 */
        sum += 2 * (seq_t)(n / 6);           /* φ(6)=2 */
    }

    return sum / (seq_t)n;
}
