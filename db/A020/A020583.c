/*
 * A020583  Smallest nonempty set S containing prime divisors of 3k+5 for each
 *          k in S.
 * https://oeis.org/A020583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Smallest nonempty set S containing prime divisors of 3k+5 for each k in S.
   The set is finite: {2, 7, 11, 13, 19, 31}. */
seq_t a(idx_t n)
{
    static const seq_t table[] = {
        (seq_t)2,
        (seq_t)7,
        (seq_t)11,
        (seq_t)13,
        (seq_t)19,
        (seq_t)31
    };
    const idx_t count = sizeof(table) / sizeof(table[0]);

    return (n > 0 && n <= count) ? table[n - 1] : (seq_t)0;
}
