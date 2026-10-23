/*
 * A160363  Numerator of Hermite(n, 5/32).
 * https://oeis.org/A160363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 5, a(n) = 5*a(n-1) - 512*(n-1)*a(n-2) for n >=
 *            2 (Numerators of Hermite(n, 5/32))
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return 5;

    seq_t prevprev = 1; /* a(0) */
    seq_t prev     = 5; /* a(1) */
    seq_t next;

    /* Iterate the linear‑recurrence up to the requested n. */
    for (idx_t i = 1; i < n; ++i) {
        /* Recurrence: a(i+1) = 5*a(i) - 512*i*a(i-1) */
        next = 5 * prev - ((seq_t)(512 * i)) * prevprev;
        prevprev = prev;
        prev = next;
    }

    return prev;
}
