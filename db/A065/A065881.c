/*
 * A065881  Ultimate modulo 10: right-hand nonzero digit of n.
 * https://oeis.org/A065881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n mod 10 if n mod 10 != 0 else a(n/10)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Last non‑zero digit of n */

seq_t a(idx_t n)
{
    /* Strip trailing zeros */
    while ((n % (idx_t)10) == (idx_t)0) {
        n = n / (idx_t)10;
    }
    /* Return the remaining last digit */
    return (seq_t)(n % (idx_t)10);
}
