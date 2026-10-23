/*
 * A024204  [ (3rd elementary symmetric function of S(n))/(2nd elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+2 odd
 *          positive integers}.
 * https://oeis.org/A024204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^3 + 2*n^2) / (3*n + 2))
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A024204 using a 128‑bit intermediate to avoid overflow */
seq_t a(idx_t n)
{
    /* If index 0 is ever queried, return 0 per convention */
    if (n == 0u) return 0;

    /* Use unsigned __int128 for safe intermediate calculations */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 num = N * N * N + (__int128)2 * N * N;
    unsigned __int128 den = (__int128)3 * N + (__int128)2;
    unsigned __int128 res = num / den;

    /* Convert back to seq_t (which may be 8/16/32/64/128 bits or an arbitrary precision type) */
    return (seq_t)res;
}
