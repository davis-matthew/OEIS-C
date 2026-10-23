/*
 * A166777  Number of nX2 1..4 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^6 + 36*n^5 + 215*n^4 + 60*n^3 - 1297*n^2 + 1164*n -
 *            180)/180
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 33, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS sequence A166777.
   n is 1‑based; for n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safety for calls below offset */

    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;          /* n^2 */
    seq_t n3 = n2 * n0;          /* n^3 */
    seq_t n4 = n3 * n0;          /* n^4 */
    seq_t n5 = n4 * n0;          /* n^5 */
    seq_t n6 = n5 * n0;          /* n^6 */

    seq_t num = (seq_t)2 * n6;               /* 2*n^6 */
    num += (seq_t)36 * n5;                  /* +36*n^5 */
    num += (seq_t)215 * n4;                 /* +215*n^4 */
    num += (seq_t)60 * n3;                  /* +60*n^3 */
    num += (seq_t)1164 * n0;                /* +1164*n */
    num -= n2 * (seq_t)1297;                /* -1297*n^2 */
    num -= (seq_t)180;                      /* -180 */

    return num / (seq_t)180;                /* division by 180 */
}
