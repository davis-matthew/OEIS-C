/*
 * A042053  Denominators of continued fraction convergents to sqrt(550).
 * https://oeis.org/A042053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A042053: denominators of continued fraction convergents to sqrt(550).
 * Recurrence: a(0)=1, a(1)=2.
 * For n>=2: let c_n be the n-th partial quotient of sqrt(550)
 *   (c_1=2, c_2=4, c_3=1, c_4=2, c_5=1, c_6=1, c_7=7, c_8=4, c_9=7,
 *    c_10=1, c_11=1, c_12=2, c_13=1, c_14=4, c_15=2, c_16=46; 
 *    thereafter c_{k+16}=c_k).
 *   Then a(n)=c_n*a(n-1)+a(n-2).
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Periodic partial quotients of sqrt(550) after the integer part */
    static const uint64_t periodic[16] = {
        2ull, 4ull, 1ull, 2ull, 1ull, 1ull,
        7ull, 4ull, 7ull, 1ull, 1ull, 2ull,
        1ull, 4ull, 2ull, 46ull
    };

    seq_t q_prevprev = (seq_t)1;  /* a(0) */
    seq_t q_prev    = (seq_t)2;  /* a(1) */
    seq_t q_curr    = q_prev;    /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        uint64_t coeff = periodic[(i - 1) % 16];
        seq_t ai = (seq_t)coeff;
        q_curr = ai * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }

    return q_curr;
}
