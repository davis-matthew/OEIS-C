/*
 * A081288  a(n) is the minimal i such that A000108(i) > n.
 * https://oeis.org/A081288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{i : Catalan(i) > n} (Catalan numbers: C(0)=1,
 *            C(i+1)=C(i)*2*(2i+1)/(i+2)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index n to seq_t for comparison */
    seq_t n_val = (seq_t)n;

    /* The first Catalan number C(0) is 1 */
    seq_t cur = (seq_t)1;          /* current Catalan value C(i)  */
    idx_t i = (idx_t)0;           /* current index i                */

    /* If n < 1, the minimal i with C(i) > n is 0 */
    if (n_val < (seq_t)1)
        return (seq_t)0;

    /* Iterate until the next Catalan number exceeds n */
    while (cur <= n_val)
    {
        /* Compute C(i+1) = C(i) * 2*(2i+1) / (i+2) */
        seq_t mult = (seq_t)(2u * (2u * i + 1u));
        cur = cur * mult / (seq_t)(i + 2u);
        ++i;
    }
    /* Return the index as seq_t */
    return (seq_t)i;
}
