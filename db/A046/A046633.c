/*
 * A046633  Number of cubic residues mod 5^n.
 * https://oeis.org/A046633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A046530(5^n) Recurrence: a(n) = 5*a(n-1) + a(n-3) -
 *            5*a(n-4) for n >= 4 Initial values: a(0)=1, a(1)=5, a(2)=21,
 *            a(3)=101
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)101;

    /* Pre‑compute constants */
    const seq_t five = (seq_t)5;

    /* Sliding window of the last four terms */
    seq_t a0 = (seq_t)1;      /* a(i-4) */
    seq_t a1 = (seq_t)5;      /* a(i-3) */
    seq_t a2 = (seq_t)21;     /* a(i-2) */
    seq_t a3 = (seq_t)101;    /* a(i-1) */
    seq_t an = 0;             /* a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        an = five * a3 + a1 - five * a0;
        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
