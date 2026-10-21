/*
 * A056594  Period 4: repeat [1,0,-1,0]; expansion of 1/(1 + x^2).
 * https://oeis.org/A056594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n%4==0, -1 if n%4==2, otherwise 0. This matches the
 *            period‑4 sequence 1,0,-1,0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n & 3U) {
        case 0: return (seq_t)1;     /* n ≡ 0 (mod 4) */
        case 2: return (seq_t)-1;    /* n ≡ 2 (mod 4) */
        default: return (seq_t)0;    /* n ≡ 1 or 3 (mod 4) */
    }
}
