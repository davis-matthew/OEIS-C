/*
 * A220997  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal or vertical neighbor.
 * https://oeis.org/A220997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 11*a(n-2) - 12*a(n-3) - 2*a(n-4) for n ≥ 5
 *            with initial values a(1)=3, a(2)=31, a(3)=306, a(4)=2989
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A220997 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)31;
    if (n == 3) return (seq_t)306;
    if (n == 4) return (seq_t)2989;

    /* For n ≥ 5 compute iteratively using the recurrence. */
    seq_t a1 = (seq_t)3;   /* a(n-4) */
    seq_t a2 = (seq_t)31;  /* a(n-3) */
    seq_t a3 = (seq_t)306; /* a(n-2) */
    seq_t a4 = (seq_t)2989;/* a(n-1) */
    seq_t ai = a4;        /* placeholder for current a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        ai =   (seq_t)11 * a4
             - (seq_t)11 * a3
             - (seq_t)12 * a2
             - (seq_t) 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return ai;
}
