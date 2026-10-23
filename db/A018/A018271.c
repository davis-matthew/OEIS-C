/*
 * A018271  Divisors of 72.
 * https://oeis.org/A018271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 72 enumerated in increasing order. */

seq_t a(idx_t n)
{
    const seq_t LIMIT = 72;
    seq_t d = 1;
    seq_t count = 0;

    for (d = 1; d <= LIMIT; ++d)
    {
        if ((LIMIT % d) == 0)
        {
            ++count;
            if (count == n)
                return d;
        }
    }
    return (seq_t)0; /* n exceeds number of divisors of 72 */
}
