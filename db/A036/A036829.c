/*
 * A036829  a(n) = Sum_{k=0..n-1} C(3*k,k)*C(3*n-3*k-2,n-k-1).
 * https://oeis.org/A036829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A036829: a(n) = Sum_{k=0..n-1} binom(3*k,k)*binom(3*n-3*k-2,n-k-1)
   Recurrence (n ≥ 3): 8*(n-1)*(2*n-1)*a(n) = 6*(36*n^2-81*n+49)*a(n-1)
                       - 81*(3*n-5)*(3*n-4)*a(n-2)
   a(0)=0, a(1)=1, a(2)=7 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle offset 0 explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    seq_t a0 = (seq_t)1;   /* a(1) */
    seq_t a1 = (seq_t)7;   /* a(2) */
    seq_t ai;

    /* Use 128‑bit arithmetic for plain integer builds (≤ 64 bits) */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = 3; i <= n; ++i) {
            __uint128_t i128 = i;
            __uint128_t left  = (__uint128_t)8 * (i128-1) * (2*i128-1);

            __uint128_t term1 = (__uint128_t)36 * i128 * i128;
            __uint128_t term2 = (__uint128_t)81 * i128;
            __uint128_t right1 = (__uint128_t)6 * (term1 - term2 + 49);

            __uint128_t temp_a = (__uint128_t)3 * i128;
            __uint128_t right2 = (__uint128_t)81 * (temp_a - 5) * (temp_a - 4);

            __uint128_t num = right1 * ( __uint128_t)a1 - right2 * ( __uint128_t)a0;
            ai = (seq_t)(num / left);
            a0 = a1;
            a1 = ai;
        }
        return a1;
    }

    /* Big‑integer build: all arithmetic done with seq_t directly */
    else {
        for (idx_t i = 3; i <= n; ++i) {
            seq_t i_seq = (seq_t)i;

            seq_t left  = (seq_t)8 * (i_seq-1) * (((seq_t)2*i_seq)-1);

            seq_t temp1 = (seq_t)36 * i_seq * i_seq;
            seq_t temp2 = (seq_t)81 * i_seq;
            seq_t right1 = (seq_t)6 * (temp1 - temp2 + 49);

            seq_t temp3 = (seq_t)3 * i_seq;
            seq_t right2 = (seq_t)81 * (temp3-5) * (temp3-4);

            seq_t num = right1*a1 - right2*a0;
            ai = num / left;
            a0 = a1;
            a1 = ai;
        }
        return a1;
    }
}
