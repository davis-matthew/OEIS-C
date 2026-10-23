/*
 * A018704  Divisors of 890.
 * https://oeis.org/A018704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 890: a(n) = the n‑th divisor of 890 (sorted ascending) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    const seq_t target = 890;
    idx_t count = 0;
    for (idx_t i = 1; i <= (idx_t)target; ++i) {
        if (target % (seq_t)i == 0) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
    return (seq_t)0;   /* n is larger than the number of divisors */
}
