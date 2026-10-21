/*
 * A056081  Numbers that are congruent to {1, 26} mod 27.
 * https://oeis.org/A056081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*floor((n-1)/2) + (n mod 2 ? 1 : 26)
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the sequence of positive integers congruent to 1 or 26 (mod 27). */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n<1; return 0 as a default. */
        return (seq_t)0;
    }

    /* m = floor((n-1)/2) */
    idx_t m = (n - 1) / 2;
    seq_t base = 27 * (seq_t)m;        /* 27*m */

    if (n & 1) {                       /* odd n -> residue 1 */
        return base + 1;
    } else {                           /* even n -> residue 26 */
        return base + 26;
    }
}
