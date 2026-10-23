/*
 * A115135  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+617)^2 = y^2.
 * https://oeis.org/A115135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 1234, n > 6 Initial values: a(1)=0,
 *            a(2)=108, a(3)=1407, a(4)=1851, a(5)=2407, a(6)=9768 Tail
 *            recursion: see implementation below
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 29, u64 n <= 65, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return directly the first six terms */
    if (n == 1) return 0;
    if (n == 2) return 108;
    if (n == 3) return 1407;
    if (n == 4) return 1851;
    if (n == 5) return 2407;
    if (n == 6) return 9768;

    /* Circular buffer holding last six values:
       d[k] = a(k+1) for k = 0..5 (mod 6) */
    seq_t d[6] = {0, 108, 1407, 1851, 2407, 9768};

    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute a(i) = 6*a(i-3) - a(i-6) + 1234
           Indices adjusted for zero‑based array: */
        seq_t term1 = d[(i - 4) % 6]; /* a(i-3) */
        seq_t term2 = d[(i - 7) % 6]; /* a(i-6) */
        seq_t next   = 6 * term1 + 1234 - term2;

        d[(i - 1) % 6] = next;       /* Store a(i) */
    }

    return d[(n - 1) % 6];
}
