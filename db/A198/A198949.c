/*
 * A198949  y-values in the solution to 11*x^2-10 = y^2.
 * https://oeis.org/A198949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 20*a(n+2)-a(n); a(1)=1, a(2)=23, a(3)=43, a(4)=461
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[4] = {1, 23, 43, 461};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not in the sequence */

    if (n <= 4) return base[n - 1];     /* fast‑path for the first four terms */

    /* Working variables hold the last four values:
     * a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)   (for the current i) */
    seq_t a0 = base[0];   /* a(1) */
    seq_t a1 = base[1];   /* a(2) */
    seq_t a2 = base[2];   /* a(3) */
    seq_t a3 = base[3];   /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 20 * a2 - a0;   /* a(i) = 20*a(i-2) - a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;   /* a(n) is stored in a3 after the loop */
}
