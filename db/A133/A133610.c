/*
 * A133610  Partial sums of pyramidal sequence A053616.
 * https://oeis.org/A133610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = SUM[i=0..n] distance from i to nearest triangular number
 * Range:     u8 n <= 107, u16 n <= 4249, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t tk = 0;                  /* T_k  */
    seq_t tk_next = 1;             /* T_{k+1} */
    idx_t k = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* advance k until T_{k+1} >= i */
        while ((seq_t)i > tk_next)
        {
            ++k;
            tk = tk_next;
            tk_next = (seq_t)(k + 1) * (seq_t)(k + 2) / 2;
        }

        seq_t id = (seq_t)i;
        seq_t d1 = id - tk;            /* distance to lower triangular */
        seq_t d2 = tk_next - id;       /* distance to upper triangular */
        sum += (d1 < d2) ? d1 : d2;    /* add nearest distance */
    }

    return sum;
}
