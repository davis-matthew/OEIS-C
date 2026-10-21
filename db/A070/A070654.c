/*
 * A070654  a(n) = n^6 mod 31.
 * https://oeis.org/A070654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 31
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n mod 31 first in the width of idx_t to avoid truncation,
       then cast the remainder to seq_t and raise it to the 6th power
       modulo 31. */
    const seq_t mod = (seq_t)31;
    const seq_t x   = (seq_t)(n % (idx_t)31);
    seq_t r = 1;
    for (int i = 0; i < 6; ++i)
        r = (seq_t)((r * x) % mod);
    return r;
}
