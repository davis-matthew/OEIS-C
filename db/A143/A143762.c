/*
 * A143762  a(n+1) = a(n)^2 + n*a(n) + n^2, a(1) = 1.
 * https://oeis.org/A143762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n+1) = a(n)^2 + n*a(n) + n^2   (for n >= 1) */
seq_t a(idx_t n)
{
    /* return 0 for out-of-range indices (not used by OEIS) */
    if (n < 1) return 0;

    seq_t result = 1;                 /* a(1) */
    for (idx_t i = 1; i < n; ++i) {   /* compute up to a(n) */
        seq_t term = result * result
                   + (seq_t)i * result
                   + (seq_t)i * (seq_t)i;
        result = term;
    }
    return result;
}
