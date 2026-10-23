/*
 * A041271  Denominators of continued fraction convergents to sqrt(148).
 * https://oeis.org/A041271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1 a(1) = 6 a(2) = 145 a(3) = 876 a(n) = 146*a(n‑2) –
 *            a(n‑4) for n ≥ 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A041271.
   Works for any sequence type seq_t (integer or arbitrary‑precision)
   and any nonnegative index type idx_t. */
seq_t a(idx_t n)
{
    /* Handle the four known initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)145;
    if (n == 3) return (seq_t)876;

    /* Keep the four last computed values:
          a0 = a(i‑4), a1 = a(i‑3), a2 = a(i‑2), a3 = a(i‑1)   */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)6;
    seq_t a2 = (seq_t)145;
    seq_t a3 = (seq_t)876;

    /* Iterate from i = 4 up to n, computing a(i) each step. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 146*a(i‑2) – a(i‑4)  ->  146*a2 – a0  */
        seq_t next = (seq_t)146 * a2 - a0;

        /* Slide the window forward. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    /* After the loop, a3 holds a(n). */
    return a3;
}
