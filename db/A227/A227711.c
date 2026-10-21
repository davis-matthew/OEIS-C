/*
 * A227711  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially in Fig. 2 of the Madanshekaf reference.
 * https://oeis.org/A227711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(254016*n^2-728028*n+1207116) +
 *            2^n*(13104*n^2-50682*n-1250685)+48039
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 18, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once */
    seq_t sqn = (seq_t)n;          /* n */
    seq_t n2  = sqn * sqn;         /* n^2 */

    /* Polynomial factors for 4^n and 2^n */
    seq_t poly1 = (seq_t)254016 * n2 - (seq_t)728028 * sqn + (seq_t)1207116;
    seq_t poly2 = (seq_t)13104  * n2 - (seq_t)50682 * sqn - (seq_t)1250685;

    /* Compute 2^n iteratively */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i)
        pow2 *= (seq_t)2;

    /* 4^n = (2^n)^2 */
    seq_t pow4 = pow2 * pow2;

    /* Final computation */
    seq_t result = pow4 * poly1 + pow2 * poly2 + (seq_t)48039;
    return result;
}
