/*
 * A116878  Generalized Catalan numbers C(2,8;n)=C(8,2;n).
 * https://oeis.org/A116878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   7*a(n)+58*a(n-1)+16*a(n-2)=144*C_{n-2}*16^{n-2} (C_k = Catalan
 *            number)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Generalized Catalan numbers C(2,8;n)=C(8,2;n)  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Use unsigned __int128 for intermediate arithmetic (fits all values needed up to n=13) */
    typedef unsigned __int128 u128;

    u128 R = (u128)144;     /* RHS for n = 2  */
    u128 a_prev2 = (u128)1; /* a(0) */
    u128 a_prev1 = (u128)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        if (i > 2) {
            /* R_i = R_{i-1} * (32*(2*i-5)) / (i-1)  */
            u128 num = R * (u128)(32 * (2 * i - 5));
            R = num / (i - 1);
        }
        u128 D = R - (u128)58 * a_prev1 - (u128)16 * a_prev2;
        u128 a_cur = D / 7;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return (seq_t)a_prev1;
}
