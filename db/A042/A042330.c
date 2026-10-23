/*
 * A042330  Numerators of continued fraction convergents to sqrt(692).
 * https://oeis.org/A042330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 17, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
  Sequence A042330: Numerators of continued fraction convergents to sqrt(692).
  Recurrence: a(n) = 4999698*a(n-14) - a(n-28) for n≥28.
  Initial terms (n=0..27):
  a(0)=26, a(1)=79, a(2)=263, a(3)=342, a(4)=947, a(5)=1289,
  a(6)=2236, a(7)=28121, a(8)=30357, a(9)=58478, a(10)=147313,
  a(11)=205791, a(12)=764686, a(13)=2499849, a(14)=130756834,
  a(15)=394770351, a(16)=1315067887, a(17)=1709838238,
  a(18)=4734744363, a(19)=6444582601, a(20)=11179326964,
  a(21)=140596506169, a(22)=151775833133, a(23)=292372339302,
  a(24)=736520511737, a(25)=1028892851039, a(26)=3823199064854,
  a(27)=12498490045601.
*/

seq_t a(idx_t n)
{
    /* The continued‑fraction coefficients for sqrt(692) after the
       initial term a0 = 26 repeat with period 14:
           [3, 3, 1, 2, 1, 1, 12, 1, 1, 2, 1, 3, 3, 52] */
    static const int period[14] = {3,3,1,2,1,1,12,1,1,2,1,3,3,52};

    /* Base convergents:
           p[-2] = 0,  p[-1] = 1    */
    seq_t p_nm2 = 0;          /* p_{k-2}   */
    seq_t p_nm1 = 1;          /* p_{k-1}   */
    seq_t p_k = 0;            /* p_k       */

    /* Compute through k = n */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t coef = (k == 0) ? (seq_t)26
                              : (seq_t)period[(k - 1) % (idx_t)14];
        p_k = coef * p_nm1 + p_nm2;  /* p_k = a_k * p_{k-1} + p_{k-2} */
        p_nm2 = p_nm1;
        p_nm1 = p_k;
    }
    return p_k;
}
