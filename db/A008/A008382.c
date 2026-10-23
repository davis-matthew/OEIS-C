/*
 * A008382  a(n) =
 *          floor(n/5)*floor((n+1)/5)*floor((n+2)/5)*floor((n+3)/5)*floor((n+4)/5).
 * https://oeis.org/A008382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            floor(n/5)*floor((n+1)/5)*floor((n+2)/5)*floor((n+3)/5)*floor((n+4)/5)
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t only for multiplication; division is fine in idx_t */
    seq_t p0 = (seq_t)(n / 5);
    seq_t p1 = (seq_t)((n + 1) / 5);
    seq_t p2 = (seq_t)((n + 2) / 5);
    seq_t p3 = (seq_t)((n + 3) / 5);
    seq_t p4 = (seq_t)((n + 4) / 5);

    seq_t res = p0;
    res *= p1;
    res *= p2;
    res *= p3;
    res *= p4;
    return res;
}
