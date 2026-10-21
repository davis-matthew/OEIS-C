/*
 * A051943  Truncated triangular pyramid numbers: a(n) = Sum_{k=9..n}
 *          (k*(k+1)/2 - 45).
 * https://oeis.org/A051943
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (n+20)*(n-8)*(n-9)/6
 * Range:     u8 n <= 15, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 9)
        return 0;           /* behaviour below offset is irrelevant */

    seq_t sn = (seq_t)n;

    /* use 128‑bit intermediate to avoid overflow before the division */
    unsigned __int128 prod =
        (unsigned __int128)(sn + 20) *
        (unsigned __int128)(sn - 8) *
        (unsigned __int128)(sn - 9);

    prod /= 6;                 /* exact division, product is always divisible by 6 */

    return (seq_t)prod;
}
