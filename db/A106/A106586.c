/*
 * A106586  Digit next to last in squares ending in 6.
 * https://oeis.org/A106586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (n mod 10)-th element of the periodic * 10‑term
 *            sequence [1,3,9,5,7,7,5,9,3,1] (period 10, offset 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic table of values.  All literals are cast to seq_t. */
    static const seq_t tbl[10] = {
        (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)5,
        (seq_t)7, (seq_t)7, (seq_t)5, (seq_t)9,
        (seq_t)3, (seq_t)1
    };

    /* a(1) must give tbl[0]; hence use (n-1) modulo 10. */
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1; safe fallback */

    idx_t r = (n - 1) % (idx_t)10;
    return tbl[r];
}
