/*
 * A153817  a(n)=Sum_{k=1..n} floor((n*k)/(n+k)).
 * https://oeis.org/A153817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} floor((n*k)/(n+k))
 * Range:     u8 n <= 16, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute S(N) = sum_{d=1..N} floor(M/d) */
static seq_t sum_floor(seq_t M, idx_t N) {
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= N) {
        seq_t q = M / (seq_t)i;          /* floor(M/i) */
        if (q == 0) break;               /* all further terms are zero */
        seq_t last_d = M / q;            /* largest d with same quotient q */
        idx_t last = (idx_t)last_d;
        if (last > N) last = N;
        sum += q * ((seq_t)(last - i + 1));
        i = last + 1;
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t M = (seq_t)n * (seq_t)n - 1;      /* M = n^2 - 1 */
    seq_t s2 = sum_floor(M, 2*n);           /* sum_{d=1}^{2n} floor((n^2-1)/d) */
    seq_t s1 = sum_floor(M, n);             /* sum_{d=1}^{n} floor((n^2-1)/d) */
    seq_t a_val = (seq_t)n * (seq_t)(n-1) - (s2 - s1);
    return a_val;
}
