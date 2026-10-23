/*
 * A026163  Sum{T(k,k-1)}, k = 1,2,...,n, where T is the array in A026148.
 * https://oeis.org/A026163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (from OEIS A026163):
 *  -(n+7)*(3*n-31)*a(n) + 3*(-n^2-35*n-76)*a(n-1)
 *  + 2*(32*n^2+27*n-459)*a(n-2) + (-47*n^2+286*n-204)*a(n-3)
 *  -3*(37*n-51)*(n-2)*a(n-4) = 0
 *  a(1)=1, a(2)=2, a(3)=6, a(4)=16. */
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Offset 1: a(0) is unused – return 0 for safety */
    if (n == 0)
        return 0;

    /* Initial values – the recurrence starts at n = 5 */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 16;

    /* Rolling window: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a_nm4 = 1;   /* a(1)  */
    seq_t a_nm3 = 2;   /* a(2)  */
    seq_t a_nm2 = 6;   /* a(3)  */
    seq_t a_nm1 = 16;  /* a(4)  */

    for (idx_t k = 5; k <= n; ++k)
    {
        /* Use signed 128‑bit for intermediate calculations to avoid overflow
         * on 64‑bit builds. The final result fits in seq_t (uint8_t … or an
         * arbitrary‑precision type) because the recurrence guarantees natural
         * numbers. */
        __int128 nn   = ( __int128 )k;
        __int128 a1   = ( __int128 )a_nm1;
        __int128 a2   = ( __int128 )a_nm2;
        __int128 a3   = ( __int128 )a_nm3;
        __int128 a4   = ( __int128 )a_nm4;

        /* Each coefficient fits in 128 bits (n up to 10^4). */
        __int128 term1 = ( 3*nn*nn + 105*nn + 228 ) * a1;
        __int128 term2 = ( 64*nn*nn + 54*nn - 918 ) * a2;
        __int128 term3 = ( 47*nn*nn - 286*nn + 204 ) * a3;
        __int128 term4 = 3 * ( 37*nn - 51 ) * ( nn - 2 ) * a4;

        /* Numerator of the recurrence (term2 is subtracted). */
        __int128 numerator = term1 - term2 + term3 + term4;

        /* Denominator: -(n+7)*(3*n-31) */
        __int128 denominator = - ( nn + 7 ) * ( 3*nn - 31 );

        /* Integer division – the result is guaranteed to be an integer. */
        __int128 next = numerator / denominator;

        /* Update rolling window for next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = (seq_t)next;
    }

    return a_nm1;
}
