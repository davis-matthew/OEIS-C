/*
 * A176514  Period 6: repeat [3, 1, 1, 3, 2, 1].
 * https://oeis.org/A176514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11/6 -cos(Pi*n/3)/6 -sqrt(3)*sin(Pi*n/3)/6
 *            +7*cos(2*Pi*n/3)/6 * +sqrt(3)*sin(2*Pi*n/3)/6 +(-1)^n/6 * which
 *            reduces to a periodic sequence with period 6: * a(1)=3, a(2)=1,
 *            a(3)=1, a(4)=3, a(5)=2, a(6)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is periodic with period 6; compute n mod 6 and map to the value */
    idx_t r = n % 6;
    switch (r) {
        case 1: return (seq_t)3;
        case 2: return (seq_t)1;
        case 3: return (seq_t)1;
        case 4: return (seq_t)3;
        case 5: return (seq_t)2;
        default: /* r == 0 (i.e., n mod 6 == 0) */
            return (seq_t)1;
    }
}
