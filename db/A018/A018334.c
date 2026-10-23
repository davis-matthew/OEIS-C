/*
 * A018334  Divisors of 207.
 * https://oeis.org/A018334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 207 in ascending order (1,3,9,23,69,207) */
seq_t a(idx_t n)
{
    static const seq_t divs[6] = {
        (seq_t)1, (seq_t)3, (seq_t)9,
        (seq_t)23, (seq_t)69, (seq_t)207
    };
    if (n < 1 || n > 6)
        return (seq_t)0;      /* undefined for out-of-range indices */
    return divs[n - 1];
}
