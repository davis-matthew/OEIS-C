/*
 * A022526  Nexus numbers (n+1)^10-n^10.
 * https://oeis.org/A022526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^10 - n^10, i.e. a(n)=∑_{k=0}^{9} C(10,k)n^k
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 106, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients C(10,k) for k = 0..9 */
    static const seq_t coeff[10] = {
        (seq_t)1,   /* C(10,0) */
        (seq_t)10,  /* C(10,1) */
        (seq_t)45,  /* C(10,2) */
        (seq_t)120, /* C(10,3) */
        (seq_t)210, /* C(10,4) */
        (seq_t)252, /* C(10,5) */
        (seq_t)210, /* C(10,6) */
        (seq_t)120, /* C(10,7) */
        (seq_t)45,  /* C(10,8) */
        (seq_t)10   /* C(10,9) */
    };

    seq_t nn = (seq_t)n;
    seq_t result = coeff[9];          /* start with highest‑degree term */

    for (int i = 8; i >= 0; --i) {
        result = result * nn + coeff[i];
    }
    return result;
}
