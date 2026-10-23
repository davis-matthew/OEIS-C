/*
 * A140058  Numbers > 24 that are congruent to {5,6,7,8,9} mod 10.
 * https://oeis.org/A140058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 25 + ((n-1) mod 5) + 10*floor((n-1)/5)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Behaviour for n < 1 is undefined for this sequence. */
        return (seq_t)0;
    }

    idx_t m = n - 1;                 /* m = n-1 (non‑negative) */
    seq_t units = (seq_t)(m % 5);    /* (n-1) mod 5   */
    seq_t tens  = ((seq_t)(m / 5)) * (seq_t)10;  /* 10 * floor((n-1)/5) */

    return (seq_t)25 + units + tens;
}
