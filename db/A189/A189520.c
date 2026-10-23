/*
 * A189520  n+[nr/t]+[ns/t]; r=1, s=1/e, t=1/(e+1).
 * https://oeis.org/A189520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*e) + floor(n/e) + 3*n
 * Range:     u8 n <= 42, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where e is the base of natural logarithms. */
/* Computation uses high‑precision integer arithmetic with a fixed rational
   approximation of e:  e ≈ P / D  and  1/e ≈ Q / D  where
       D = 10^15
       P = floor(e*D) = 2718281828459045
       Q = floor((1/e)*D) = 367879441171442
   The error of each approximation is less than 1/D, which is
   smaller than 1/n for all n considered (n < D).  Thus,
   floor(n*e) = I + (R + n >= D ? 1 : 0)   with
       I = (n*P)/D,  R = (n*P)%D
   Similarly  floor(n/e) = J + (S + n >= D ? 1 : 0)
       J = (n*Q)/D,  S = (n*Q)%D
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for P/Q/D */
    const __int128 D = 1000000000000000LL;          /* 1e15 */
    const __int128 P = 2718281828459045LL;          /* floor(e*D) */
    const __int128 Q = 367879441171442LL;          /* floor((1/e)*D) */

    if (n == 0) return (seq_t)0; /* harmless when called with offset 0 */

    /* floor(n*e) */
    __int128 prod_e = (__int128)n * P;
    seq_t fe = (seq_t)(prod_e / D);
    __int128 rem_e = prod_e % D;
    if (rem_e + n >= D) fe++;

    /* floor(n/e) */
    __int128 prod_c = (__int128)n * Q;
    seq_t fc = (seq_t)(prod_c / D);
    __int128 rem_c = prod_c % D;
    if (rem_c + n >= D) fc++;

    /* a(n) = floor(n*e) + floor(n/e) + 3*n */
    seq_t result = fe + fc + (seq_t)3 * (seq_t)n;
    return result;
}
