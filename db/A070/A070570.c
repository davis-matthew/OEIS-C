/*
 * A070570  a(n) = n^4 mod 27.
 * https://oeis.org/A070570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 27
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 27 first to keep intermediate values tiny.
       Afterwards square twice modulo 27 to avoid any overflow. */
    seq_t r  = (seq_t)n % 27;
    seq_t r2 = (r * r) % 27;
    seq_t r4 = (r2 * r2) % 27;
    return r4;
}
