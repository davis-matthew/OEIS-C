/*
 * A024169  Integer part of ((2nd elementary symmetric function of
 *          1,2,...,n)/(1+2+...+n)).
 * https://oeis.org/A024169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1/12)*(n-1)*(3*n + 2))
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a 128‑bit intermediate to avoid overflow when n is large */
    unsigned __int128 prod = (unsigned __int128)(n - 1) *
                            ((unsigned __int128)3 * n + 2);
    return (seq_t)(prod / 12);
}
