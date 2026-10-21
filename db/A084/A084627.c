/*
 * A084627  a(n) = floor(C(n+6,6)/C(n+3,3)).
 * https://oeis.org/A084627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n+4)(n+5)(n+6) / 120)
 * Range:     u8 n <= 26, u16 n <= 193, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to floor(binomial(n+6,3) / 20) or the given polynomial formula. */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow for typical testcase ranges. */
    __int128 v   = n;                     /* cast the index once */
    __int128 prod = (v + 4) * (v + 5) * (v + 6);
    __int128 res  = prod / 120;           /* integer division gives floor for positives */
    return (seq_t)res;
}
