/*
 * A028215  Expansion of 1/((1-6x)(1-8x)(1-11x)(1-12x)).
 * https://oeis.org/A028215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=37; for n>=2:
 *            a(n)=23*a(n-1)-132*a(n-2)+2^n*(4^(n+1)-3^(n+1))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)37;
    seq_t a_prev = (seq_t)1;
    seq_t a_curr = (seq_t)37;
    seq_t p2 = 1; /* 2^0 */
    seq_t p4 = 1; /* 4^0 */
    seq_t p3 = 1; /* 3^0 */
    for (idx_t i = 1; i <= n; ++i) {
        /* advance powers to exponent i */
        p2 *= (seq_t)2;
        p4 *= (seq_t)4;
        p3 *= (seq_t)3;
        if (i < 2) continue; /* recurrence starts at n=2 */
        seq_t term = p2 * ((p4 * (seq_t)4) - (p3 * (seq_t)3));
        seq_t new_a = (seq_t)23 * a_curr - (seq_t)132 * a_prev + term;
        a_prev = a_curr;
        a_curr = new_a;
    }
    return a_curr;
}
