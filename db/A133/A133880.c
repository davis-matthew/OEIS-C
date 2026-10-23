/*
 * A133880  n modulo p repeated p times (where p=10).
 * https://oeis.org/A133880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+⌊n/10⌋) mod 10
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/10) + 1, then take modulo 10.
       The intermediate result fits in idx_t, so casting
       to seq_t after the modulo guarantees a value
       in the range 0..9. */
    return (seq_t)((n / 10 + 1) % 10);
}
