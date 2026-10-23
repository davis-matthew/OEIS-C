/*
 * A108677  a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(19*n^2 + 47*n + 30)/720.
 * https://oeis.org/A108677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(19*n^2 + 47*n + 30)/720
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute in unsigned 128‑bit to avoid overflow on intermediate steps */
    unsigned __int128 N  = (unsigned __int128)n;
    unsigned __int128 t1 = N + 1;
    unsigned __int128 t2 = N + 2;
    unsigned __int128 t3 = N + 3;
    unsigned __int128 t4 = N + 4;

    /* product (n+1)(n+2)(n+3)(n+4) / 24  = binomial(n+4,4)  */
    unsigned __int128 prod = t1 * t2 * t3 * t4;
    prod /= 24;                           /* exact division */

    /* polynomial 19*n^2 + 47*n + 30 */
    unsigned __int128 poly = N * N;        /* n^2   */
    poly *= 19;                           /* 19*n^2 */
    poly += N * 47;                       /* + 47*n */
    poly += 30;                           /* + 30   */

    prod *= poly;                         /* intermediate may be large */
    prod /= 30;                           /* exact division by 30 */

    return (seq_t)prod;                   /* cast to the build’s integer type */
}
