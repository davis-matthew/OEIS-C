/*
 * A132139  a(n) = f(n,1) with f(m,x) = if m<3 then (if m=1 then x else 0)
 *          else (if m mod 3 = 0 then 0 else f([m/3], x*(1 + m mod 3) + 1)).
 * https://oeis.org/A132139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n,1) where f(m,x) = m<3 ? (m==1?x:0) :
 *            (m%3==0?0:f([m/3], x*(1 + m%3)+1)) (offset = 1,
 *            a(1)=1,a(2)=0,a(3)=0,…)
 * Range:     u8 n <= 481, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1;
    idx_t m = n;

    while (m >= 3) {
        uint32_t r = (uint32_t)(m % 3);
        if (r == 0) return 0;
        x = x * (1 + r) + 1;
        m /= 3;
    }
    return (m == 1) ? x : 0;
}
