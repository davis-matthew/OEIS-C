/*
 * A060470  Smallest positive a(n) such that number of solutions to
 *          a(n)=a(j)+a(k) j<k<n is two or less.
 * https://oeis.org/A060470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 3*floor((n+2)/3) - 17 (for n > 9)
 * Range:     u8 n <= 90, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base values: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=8, a(8)=10, a(9)=12.  */

seq_t a(idx_t n) {
    static const seq_t base[] = {1, 2, 3, 4, 5, 6, 8, 10, 12};
    if (n <= 9) {
        return base[n - 1];
    }
    /* Compute using the closed‑form formula.  All arithmetic is performed
       in seq_t to avoid overflow on the temporary values. */
    seq_t nn = (seq_t)n;
    seq_t term = 2 * nn + 3 * ((nn + 2) / 3) - 17;
    return term;
}
