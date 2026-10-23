/*
 * A185138  a(4*n) = n*(4*n-1); a(2*n+1) = n*(n+1)/2; a(4*n+2) =
 *          (2*n+1)*(4*n+1).
 * https://oeis.org/A185138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(4*n) = n*(4*n-1); a(2*n+1) = n*(n+1)/2; a(4*n+2) =
 *            (2*n+1)*(4*n+1);
 * Range:     u8 n <= 25, u16 n <= 365, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* odd indices: n = 2*k+1 */
    if (n & 1U) {
        idx_t k = n >> 1;                /* k = (n-1)/2 */
        seq_t sk = (seq_t)k;
        return (sk * (sk + 1)) / (seq_t)2;
    }

    /* even indices: distinguish n % 4 */
    if ((n & 3U) == 0U) {                 /* n = 4*t */
        idx_t t = n >> 2;
        seq_t st = (seq_t)t;
        return st * ((st * (seq_t)4) - (seq_t)1);
    }

    /* n % 4 == 2 : n = 4*t+2 */
    idx_t t = n >> 2;                      /* t = (n-2)/4 */
    seq_t st = (seq_t)t;
    return ((st * (seq_t)2) + (seq_t)1) * ((st * (seq_t)4) + (seq_t)1);
}
