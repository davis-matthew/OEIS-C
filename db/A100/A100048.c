/*
 * A100048  A Chebyshev transform of the Pell numbers.
 * https://oeis.org/A100048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4), initial values:
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 71, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A100048. */
seq_t a(idx_t n)
{
    /* Small constants used in the recurrence. */
    static const seq_t zero = 0;
    static const seq_t one  = 1;
    static const seq_t two  = 2;

    /* Handle the first four known terms directly. */
    if (n == 0) return zero;
    if (n == 1) return one;
    if (n == 2) return two;
    if (n == 3) return two;

    /* Iterative computation using a sliding 4‑term window. */
    seq_t a0 = zero;  /* a(n-4)   */
    seq_t a1 = one;   /* a(n-3)   */
    seq_t a2 = two;   /* a(n-2)   */
    seq_t a3 = two;   /* a(n-1)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - a(i-2) + 2*a(i-3) - a(i-4) */
        next = two * a3 - a2 + two * a1 - a0;

        /* Slide the window for the next step. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
