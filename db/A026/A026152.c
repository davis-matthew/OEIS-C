/*
 * A026152  a(n) = T(n,n-1), where T is the array in A026148.
 * https://oeis.org/A026152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [ (164*n^2+313*n-2115)*a(n-1) +
 *            (-182*n^2+229*n+891)*a(n-2) + (-164*n^2+59*n+519)*a(n-3) +
 *            3*(71*n-104)*(n-3)*a(n-4) ] / ((n+7)*(31*n-87)) Initial values:
 *            a(2)=1, a(3)=4, a(4)=10, a(5)=29
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 14, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n<2, return 0 */

    /* handle the four base terms */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)10;
    if (n == 5) return (seq_t)29;

    /* sliding window for the 4 previous terms */
    seq_t a_nm4 = (seq_t)1;   /* a(2) */
    seq_t a_nm3 = (seq_t)4;   /* a(3) */
    seq_t a_nm2 = (seq_t)10;  /* a(4) */
    seq_t a_nm1 = (seq_t)29;  /* a(5) */

    for (idx_t i = 6; i <= n; ++i)
    {
        /* compute integer coefficients using 128-bit intermediate */
        uint64_t coeff1 = (uint64_t)(
            ((__uint128_t)164 * i * i + (__uint128_t)313 * i - 2115));
        uint64_t coeff2pos = (uint64_t)(
            ((__uint128_t)182 * i * i - (__uint128_t)229 * i - 891));
        uint64_t coeff3pos = (uint64_t)(
            ((__uint128_t)164 * i * i - (__uint128_t)59 * i - 519));
        uint64_t coeff4 = (uint64_t)(
            (__uint128_t)3 * (71 * i - 104) * (i - 3));
        uint64_t denom_val = (uint64_t)(i + 7) * (31 * i - 87);

        /* apply the recurrence */
        seq_t term1 = a_nm1 * (seq_t)coeff1;            /* positive term */
        seq_t term2 = a_nm2 * (seq_t)coeff2pos;         /* magnitude of negative term */
        seq_t term3 = a_nm3 * (seq_t)coeff3pos;         /* magnitude of negative term */
        seq_t term4 = a_nm4 * (seq_t)coeff4;            /* positive term */

        seq_t pos = term1 + term4;
        seq_t neg = term2 + term3;
        seq_t numerator = pos - neg;                    /* ensured non‑negative */

        seq_t a_new = numerator / (seq_t)denom_val;     /* exact division */

        /* shift the window for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_new;
    }

    return a_nm1;  /* result for requested n */
}
