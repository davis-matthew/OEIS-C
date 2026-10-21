/*
 * A063222  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          58 ).
 * https://oeis.org/A063222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A063222 – dimension of the space of weight 2n cuspidal newforms for Γ₀(58).
    The sequence satisfies:
        a(1)=2, a(2)=7, a(3)=13, a(4)=15
        for n≥5:
            let k = floor((n-4)/3), r = (n-4) mod 3
            a(n) = 15 + 14*k + (r==1 ? 6 : (r==2 ? 12 : 0))
*/
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)15;

    /* For n ≥ 5 */
    idx_t k = (n - 4) / 3;
    idx_t r = (n - 4) % 3;
    seq_t result = (seq_t)15 + (seq_t)14 * (seq_t)k;

    if (r == 1)
        result += (seq_t)6;
    else if (r == 2)
        result += (seq_t)12;

    return result;
}
