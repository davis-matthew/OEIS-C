/*
 * A156619  Numbers congruent to {7, 18} mod 25.
 * https://oeis.org/A156619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25 * floor((n-1)/2) + (n%2==1 ? 7 : 18), n >= 1
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A156619 (numbers ≡ {7,18} mod 25). */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, but guard against zero just in case */
        return 0;
    idx_t half = (n - 1) >> 1;           /* floor((n-1)/2)  */
    seq_t base = (seq_t)half * (seq_t)25;/* 25 * floor((n-1)/2)  */
    return base + (seq_t)((n & 1) ? 7 : 18);
}
