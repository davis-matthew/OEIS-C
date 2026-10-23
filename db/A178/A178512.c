/*
 * A178512  Reversed decimal expansions of A178510.
 * https://oeis.org/A178512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 120*a(n-1) + 1, with a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS sequence A178512) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* illegal index, safety */

    const seq_t coef = (seq_t)120;            /* multiplier in the recurrence */
    seq_t value = (seq_t)1;                   /* a(1) */

    /* compute a(n) by iterating the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        /* value = 120 * value + 1;   all operations in seq_t */
        value = value * coef + (seq_t)1;
    }

    return value;
}
