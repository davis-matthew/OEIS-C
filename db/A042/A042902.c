/*
 * A042902  Numerators of continued fraction convergents to sqrt(983).
 * https://oeis.org/A042902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of the continued‑fraction convergents to
 *            sqrt(983) a(0)=31, a(1)=63, a(2)=94, a(3)=533, a(4)=16617,
 *            a(5)=83618, a(6)=100235, a(7)=284088; they satisfy
 *            a(n)=568176*a(n-8)-a(n-16) for n≥16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The continued–fraction terms of sqrt(983) are periodic with period 8:
       first term a0 = 31, then the period (a1 … a8) is
       2,1,5,31,5,1,2,62 (=2*a0).  */
    static const seq_t period[8] = {2, 1, 5, 31, 5, 1, 2, 62};

    seq_t prev2 = (seq_t)0;   /* p_{k-2} */
    seq_t prev1 = (seq_t)1;   /* p_{k-1} */
    seq_t cur;                /* p_k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak;
        if (k == 0) {
            ak = (seq_t)31;          /* a0 */
        } else {
            /* a_k = period[(k-1) mod 8] for k>=1 */
            ak = period[(k - 1) & 7U];
        }
        cur = ak * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
