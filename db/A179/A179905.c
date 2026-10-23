/*
 * A179905  (1, 4, 7, 10, 13, ...) convolved with (1, 0, 4, 7, 10, 13, ...);
 *          given A016777 = (1, 4, 7, 10, 13, ...).
 * https://oeis.org/A179905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4),
   initial values: a(0)=1, a(1)=4, a(2)=11, a(3)=33, a(4)=79 */
seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)33;
    if (n == 4) return (seq_t)79;

    /* Initialise a sliding window with a(1) .. a(4)   */
    seq_t a0 = (seq_t)4;   /* a(n-4) */
    seq_t a1 = (seq_t)11;  /* a(n-3) */
    seq_t a2 = (seq_t)33;  /* a(n-2) */
    seq_t a3 = (seq_t)79;  /* a(n-1) */
    seq_t an = 0;          /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)4 * a3 - (seq_t)6 * a2 + (seq_t)4 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
