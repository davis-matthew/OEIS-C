/*
 * A059620  Colors of the 88 keys of the standard piano: white keys = 0, black
 *          keys = 1, start with A0 = the 0th key.
 * https://oeis.org/A059620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn, nice
 * Formula:   a(n) = floor((5*n+7)/12) - floor((5*n+2)/12)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This reduces to a(n) = 1 if (5*n+2) mod 12 ≥ 7, else 0. */
seq_t a(idx_t n)
{
    /* Compute (5*n + 2) mod 12 without overflow. */
    idx_t remainder = ((n % 12) * 5 + 2) % 12;
    return (remainder >= 7) ? (seq_t)1 : (seq_t)0;
}
