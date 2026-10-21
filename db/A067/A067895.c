/*
 * A067895  Write 2^n, 2^n+1, 2^n+2, ..., 2^(n+1)-1 in binary and add as if
 *          they were decimal numbers.
 * https://oeis.org/A067895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 22*a(n-1) - 40*a(n-2) with a(0) = 1, a(1) = 21
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS sequence A067895 for any n >= 0.
   The calculation uses the linear recurrence from the OEIS entry,
   ensuring exact integer arithmetic for the types used by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;

    /* Use only seq_t arithmetic. */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)21; /* a(1) */
    seq_t an = a0;        /* placeholder */

    /* Iterate with the two-term recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)22 * a1 - (seq_t)40 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
