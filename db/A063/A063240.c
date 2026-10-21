/*
 * A063240  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          89 ).
 * https://oeis.org/A063240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7; for n≥2, a(n)=7+15+44·floor((n-2)/3)+
 *            (rem==1?14:0)+(rem==2?16:0), where rem = (n-2) mod 3. This comes
 *            from the linear recurrence a(n)=a(n-1)+a(n-3)-a(n-4) with
 *            initial values 7,22,36,52
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, safe fallback */
    if (n == 1) return 7;

    /* base for n≥2 */
    seq_t result = 7 + 15;          /* 7 + (a(2)-a(1)) */

    idx_t k = n - 2;                /* number of steps after the first difference */
    idx_t cycles = k / 3;           /* full 3‑step cycles */
    idx_t rem    = k % 3;           /* remaining steps in current cycle */

    result += (seq_t)cycles * 44;   /* each full cycle adds 14+16+14 = 44 */

    if (rem >= 1) result += 14;     /* first remaining step adds 14 */
    if (rem == 2) result += 16;     /* second remaining step adds another 16 */

    return result;
}
