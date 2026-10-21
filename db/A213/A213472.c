/*
 * A213472  Period 20, repeat [1, 4, 0, 9, 1, 6, 4, 5, 9, 6, 6, 9, 5, 4, 6, 1,
 *          9, 0, 4, 1].
 * https://oeis.org/A213472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ((3*n*n + 3*n + 2)/2) mod 10
 *              (units digit of the centered triangular number 3*n*(n+1)/2 + 1)
 */
seq_t a(idx_t n)
{
    /* Reduce n modulo 20, because the sequence has period 20. */
    idx_t n20 = n % 20ULL;

    /* Compute (3*n20*n20 + 3*n20 + 2) modulo 20.
       All intermediate values fit in 64 bits (n20 <= 19). */
    unsigned long long tmp = (3ULL * n20 * n20 + 3ULL * n20 + 2ULL) % 20ULL;

    /* tmp is guaranteed to be even, so division by 2 is safe and
       yields the units digit (0–9). */
    seq_t digit = (seq_t)(tmp / 2ULL);
    return digit;
}
