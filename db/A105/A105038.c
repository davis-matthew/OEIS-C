/*
 * A105038  Nonnegative n such that 6*n^2 + 6*n + 1 is a square.
 * https://oeis.org/A105038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105038: a(n) = 11*a(n-1) - 11*a(n-2) + a(n-3)
   with a(0)=0, a(1)=4, a(2)=44 (OEIS offset 0). */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)4;
    if (n == 2u) return (seq_t)44;

    /* Recurrence variables: a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)0;      /* a(0) */
    seq_t a1 = (seq_t)4;      /* a(1) */
    seq_t a2 = (seq_t)44;     /* a(2) */
    seq_t eleven = (seq_t)11;
    seq_t a_next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 11*a(i-1) - 11*a(i-2) + a(i-3) */
        a_next = a2 * eleven - a1 * eleven + a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
