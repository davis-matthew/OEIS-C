/*
 * A163317  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+4*a(n-2)+4*a(n-3)+4*a(n-4)-10*a(n-5) for n ≥ 6
 *            Initial values: a(0) = 1, a(1) = 6, a(2) = 30, a(3) = 150, a(4)
 *            = 750, a(5) = 3735. The function returns a(n) for every n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)30;
    if (n == 3) return (seq_t)150;
    if (n == 4) return (seq_t)750;
    if (n == 5) return (seq_t)3735;

    /* For n ≥ 6, use the recurrence. Initialize the sliding window with
       a(1) … a(5) so that for i = 6 the recurrence uses the correct indices. */
    seq_t a_m5 = (seq_t)6;     /* a(i-5) */
    seq_t a_m4 = (seq_t)30;    /* a(i-4) */
    seq_t a_m3 = (seq_t)150;   /* a(i-3) */
    seq_t a_m2 = (seq_t)750;   /* a(i-2) */
    seq_t a_m1 = (seq_t)3735;  /* a(i-1) */
    seq_t next;

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) using the recurrence. All arithmetic is performed
           in seq_t, preserving precision for arbitrary‑precision builds
           and avoiding overflow on 64‑bit builds until the value no
           longer fits. */
        next = (seq_t)4 * a_m1 + (seq_t)4 * a_m2 + (seq_t)4 * a_m3
             + (seq_t)4 * a_m4 - (seq_t)10 * a_m5;

        /* Slide the window: drop a_m5, shift each element, and insert next. */
        a_m5 = a_m4;
        a_m4 = a_m3;
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = next;
    }

    return a_m1;
}
