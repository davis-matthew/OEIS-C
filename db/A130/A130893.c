/*
 * A130893  Lucas numbers (beginning with 1) mod 10.
 * https://oeis.org/A130893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Lucas(n+1) mod 10, period 12 a(0)=1, a(1)=3, a(2)=4,
 *            a(3)=7, a(4)=1, a(5)=8, a(6)=9, a(7)=7, a(8)=6, a(9)=3, a(10)=9,
 *            a(11)=2 (information from OEIS A130893)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t periodic[12] = {
        (seq_t)1,  (seq_t)3,  (seq_t)4,  (seq_t)7,
        (seq_t)1,  (seq_t)8,  (seq_t)9,  (seq_t)7,
        (seq_t)6,  (seq_t)3,  (seq_t)9,  (seq_t)2
    };
    /* The sequence is periodic with period 12, so we only need
       n modulo 12 to determine the value. */
    idx_t idx = n % 12;
    return periodic[idx];
}
