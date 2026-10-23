/*
 * A159742  If an array is made of columns of -nacci sequences (Fibonacci,
 *          tribonacci, etc.), all starting with 1,1,2,..., the NW-to-SE
 *          diagonals can be extended by computation. This sequence is
 *          diagonal 6. See A159741 for details.
 * https://oeis.org/A159742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) for n>3; Initial values: a(1) = 13,
 *            a(2) = 44, a(3) = 108
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the base cases directly. */
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)44;
    if (n == 3) return (seq_t)108;

    /* Seeds for the recurrence: a1 = a(n-2), a2 = a(n-1) at n = 4. */
    seq_t a1 = (seq_t)44;   /* a(2) */
    seq_t a2 = (seq_t)108;  /* a(3) */
    seq_t an = a2;          /* will hold a(n) */

    /* Apply the linear recurrence for n >= 4. */
    for (idx_t i = 4; i <= n; ++i) {
        an = 3 * a2 - 2 * a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
