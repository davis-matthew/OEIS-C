/*
 * A119032  a(n+2) = 18*a(n+1) - a(n) + 8.
 * https://oeis.org/A119032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+2) = 18*a(n+1) – a(n) + 8,  a(1) = 0,  a(2) = 9 */
seq_t a(idx_t n)
{
    /* a(1) and a(2) are the base cases of the recurrence. */
    seq_t a_val = (seq_t)0;          /* a(1) */
    seq_t b_val = (seq_t)9;          /* a(2) */
    if (n == (idx_t)1)
        return a_val;
    if (n == (idx_t)2)
        return b_val;

    /* Iterate the linear recurrence. */
    for (idx_t i = (idx_t)3; i <= n; ++i)
    {
        seq_t c = (seq_t)18 * b_val - a_val + (seq_t)8;
        a_val = b_val;  /* shift: previous a becomes old b */
        b_val = c;      /* new term becomes the current b */
    }

    return b_val; /* b_val holds a(n) after the loop */
}
