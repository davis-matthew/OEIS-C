/*
 * A203684  v(n+1)/v(n), where v=A203683.
 * https://oeis.org/A203684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5; Recurrence: a(n+1) = (4^n + 4^(2*n+1)) * a(n) for n ≥
 *            1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* The first term a(1) = 5. */
        return (seq_t)5;
    }

    seq_t val = (seq_t)5;           /* a(1) */
    seq_t pow4_n   = (seq_t)4;      /* 4^1 */
    seq_t pow4_2n1 = (seq_t)64;     /* 4^(2*1+1) = 4^3 */

    for (idx_t k = 1; k < n; ++k) {        /* compute up to a(n) */
        seq_t prod = pow4_n + pow4_2n1;     /* 4^k + 4^(2k+1)      */
        val = val * prod;                  /* val = a(k+1)         */

        /* advance 4-powers for the next iteration:
           4^(k+1)  = 4^k * 4
           4^(2(k+1)+1) = 4^(2k+1) * 4^2 = *16   */ 
        pow4_n   *= (seq_t)4;
        pow4_2n1 *= (seq_t)16;
    }
    return val;
}
