/*
 * A151611  a(1) = 4; for n >= 1, a(n+1) = 4 + binomial(a(n), 2).
 * https://oeis.org/A151611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 4; a(n+1) = 4 + binomial(a(n), 2) */
seq_t a(idx_t n)
{
    /* The sequence is well defined for n >= 1.
       Values for n < 1 are not required; we return 0. */
    if (n == 0)
        return 0;

    seq_t val = (seq_t)4;          /* a(1) */
    if (n == 1)
        return val;

    /* Iterate the recurrence up to n. */
    for (idx_t i = 1; i < n; ++i) {
        /* binomial(a,2) = a*(a-1)/2 */
        seq_t tmp = (val * (val - (seq_t)1)) / (seq_t)2;
        val = (seq_t)4 + tmp;
    }
    return val;
}
