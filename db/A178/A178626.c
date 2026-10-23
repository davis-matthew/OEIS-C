/*
 * A178626  Convolution of Pell(n) and 10^n.
 * https://oeis.org/A178626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+Pell(n), a(1)=1. Pell numbers: Pell(0)=0,
 *            Pell(1)=1, Pell(k)=2*Pell(k-1)+Pell(k-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for 0, return 0 */

    seq_t pell_prev = 0;   /* Pell(0) */
    seq_t pell_curr = 1;   /* Pell(1) */
    seq_t result = 1;      /* a(1) */

    if (n == 1) return result;

    for (idx_t i = 2; i <= n; ++i) {
        /* compute Pell(i) */
        seq_t pell_next = 2 * pell_curr + pell_prev;
        pell_prev = pell_curr;
        pell_curr = pell_next;
        /* a(i) = 10*a(i-1)+Pell(i) */
        result = 10 * result + pell_curr;
    }
    return result;
}
