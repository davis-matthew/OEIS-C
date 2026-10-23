/*
 * A158764  a(n) = 38*(38*n^2 - 1).
 * https://oeis.org/A158764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*(38*n^2 - 1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* cast n to seq_t first to avoid 64‑bit overflow in intermediate steps
       and guarantee exact arithmetic for values that fit in seq_t. */
    seq_t tn = (seq_t)n;
    seq_t t = 38 * tn * tn;      /* 38*n^2  */
    t -= 1;                       /* 38*n^2 - 1 */
    t *= 38;                      /* 38*(38*n^2 - 1) */
    return t;
}
