/*
 * A223574  Number of nX6 0..2 arrays with antidiagonals unimodal.
 * https://oeis.org/A223574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: let U(n)= (n^4+6*n^3+23*n^2+18*n+24)/24.
 * a(n)=U(min(n,6))^(max(n,6)-min(n,6)+1) *
 *      Π_{i=1}^{min(n,6)-1} U(i)^2
 * For n>5, a(n)=148*a(n-1).
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* U(1..6) */
    static const seq_t U[7] = {0,
                               (seq_t)3,(seq_t)9,(seq_t)22,(seq_t)46,(seq_t)86,(seq_t)148};
    /* product_{i=1}^{k} U(i)^2  for k=0..5 */
    static const seq_t prodSq[6] = {
        (seq_t)1,                               /* k=0 */
        (seq_t)9,                               /* 3^2 */
        (seq_t)729,                             /* 9^2 = 81, times 9 = 729 */
        (seq_t)352836,                          /* 22^2 = 484 * previous 729 = 352836 */
        (seq_t)746600976,                       /* 46^2 = 2116 * previous = 746600976 */
        (seq_t)5521860818496ULL                 /* 86^2 = 7396 * previous */
    };

    if (n < 1) {
        return (seq_t)0;   /* Offset is 1; behaviour for n<1 is irrelevant */
    }

    if (n <= 6) {
        seq_t result = prodSq[n - 1];
        seq_t power = 1;
        idx_t exp = 7 - n;          /* exponent for U(n) */
        for (idx_t i = 0; i < exp; ++i) {
            power *= U[n];
        }
        result *= power;
        return result;
    }

    /* n > 6: use the recurrence a(n)=148*a(n-1) */
    seq_t result = prodSq[5] * (seq_t)148;  /* a(6) */
    for (idx_t i = 7; i <= n; ++i) {
        result *= (seq_t)148;
    }
    return result;
}
