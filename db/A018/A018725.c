/*
 * A018725  Divisors of 927.
 * https://oeis.org/A018725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 927 in ascending order */
seq_t a(idx_t n)
{
    /* The known divisors of 927 = 3^2 * 103 */
    static const seq_t divisors[6] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)9,
        (seq_t)103,
        (seq_t)309,
        (seq_t)927
    };
    if (n < 1) return (seq_t)0;
    if (n <= 6) return divisors[n - 1];
    return (seq_t)0;  /* undefined for n > 6 */
}
