/*
 * A054145  Row sums of array T as in A054144.
 * https://oeis.org/A054145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-2)*((2+sqrt(2))^n + (2-sqrt(2))^n) +
 *            sqrt(2)*((2+sqrt(2))^n - (2-sqrt(2))^n))/8 Recurrence (derived
 *            from the g.f.): a(0)=0, a(1)=2, a(2)=12, a(3)=58, for n≥4:
 *            a(n)=8*a(n-1)−20*a(n-2)+16*a(n-3)−4*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external includes are necessary because the only types used are
   idx_t and seq_t which are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return 0;
    if (n == 1) return 2;
    if (n == 2) return 12;
    if (n == 3) return 58;

    /* Variables to hold the last four terms of the sequence. */
    seq_t a0 = 0, a1 = 2, a2 = 12, a3 = 58, an = 0;

    /* Iterate from 4 up to n using the recurrence. */
    for (idx_t i = 4; i <= n; ++i) {
        an = 8 * a3;          /* 8*a(n-1) */
        an -= 20 * a2;        /* minus 20*a(n-2) */
        an += 16 * a1;        /* plus 16*a(n-3) */
        an -= 4 * a0;         /* minus 4*a(n-4) */

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
