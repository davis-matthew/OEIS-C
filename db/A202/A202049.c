/*
 * A202049  Number of (n+2) X 7 binary arrays avoiding patterns 001 and 110 in
 *          rows and columns.
 * https://oeis.org/A202049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/180)*n^7 + (7/36)*n^6 + (511/180)*n^5 + (805/36)*n^4 +
 *            (4606/45)*n^3 + (2443/9)*n^2 + (1854/5)*n + 196
 * Range:     u8 none, u16 none, u32 n <= 18, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to avoid repeated casts */
    seq_t nseq  = (seq_t)n;
    seq_t n2    = nseq * nseq;          /* n^2 */
    seq_t n3    = n2   * nseq;          /* n^3 */
    seq_t n4    = n3   * nseq;          /* n^4 */
    seq_t n5    = n4   * nseq;          /* n^5 */
    seq_t n6    = n5   * nseq;          /* n^6 */
    seq_t n7    = n6   * nseq;          /* n^7 */

    seq_t N = n7;                       /* start with n^7 term */

    N += n6 * (seq_t)35;                /* +35*n^6   */
    N += n5 * (seq_t)511;               /* +511*n^5  */
    N += n4 * (seq_t)4025;              /* +4025*n^4 */
    N += n3 * (seq_t)18424;             /* +18424*n^3*/
    N += n2 * (seq_t)48860;             /* +48860*n^2*/
    N += nseq * (seq_t)66744;           /* +66744*n  */
    N += (seq_t)35280;                  /* +35280    */

    return N / (seq_t)180;
}
