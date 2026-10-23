/*
 * A145542  Numerators in continued fraction expansion of sqrt(3/5).
 * https://oeis.org/A145542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A145542 – Numerators in continued fraction expansion of sqrt(3/5)
 *
 * Recurrence: a(1)=1, a(2)=3, a(3)=7, a(4)=24,
 *            a(n)=8*a(n-2)-a(n-4)   for n>=5.
 *
 * The function computes a(n) directly in O(n) time using only
 * integer arithmetic.  This works for any seq_t and idx_t defined by
 * the build platform, including arbitrary‑precision integer classes.
 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined, but harmless */

    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)24;

    /* Maintain a sliding window of a(n-4), a(n-3), a(n-2), a(n-1). */
    seq_t a_minus4 = (seq_t)1;      /* a(1) */
    seq_t a_minus3 = (seq_t)3;      /* a(2) */
    seq_t a_minus2 = (seq_t)7;      /* a(3) */
    seq_t a_minus1 = (seq_t)24;     /* a(4) */
    seq_t current;

    for (idx_t i = 5; i <= n; ++i) {
        current = (seq_t)8 * a_minus2 - a_minus4;  /* a(i) = 8*a(i-2)-a(i-4) */

        /* Shift window forward */
        a_minus4 = a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = current;
    }

    return current;
}
