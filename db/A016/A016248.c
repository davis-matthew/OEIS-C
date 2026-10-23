/*
 * A016248  Expansion of 1/((1-x)(1-6x)(1-12x)).
 * https://oeis.org/A016248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 19, a(n) = 18*a(n-1) - 72*a(n-2) + 1 for n ≥ 2
 *            (sequence A016248)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)19;

    /* Iteratively compute using the recurrence relation */
    seq_t a_nm2 = (seq_t)1;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)19; /* a(n-1) */
    seq_t an;                /* current a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)18 * a_nm1 - (seq_t)72 * a_nm2 + (seq_t)1;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return an;
}
