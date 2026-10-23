/*
 * A152504  1/10 of the number of permutations of 4 indistinguishable copies
 *          of 1..n with exactly 2 local maxima.
 * https://oeis.org/A152504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*35^(n-1) - 11*5^(n-1) - 12*(n-1)*5^(n-1))/90;
 *            Recurrence: a(n) = 45*a(n-1) - 375*a(n-2) + 875*a(n-3) for n>3
 *            Initial values: a(1)=0, a(2)=3, a(3)=140
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 3;
    if (n == 3) return 140;

    seq_t a1 = 140; /* a(n-1) */
    seq_t a2 = 3;   /* a(n-2) */
    seq_t a3 = 0;   /* a(n-3) */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        curr = (seq_t)45 * a1 - (seq_t)375 * a2 + (seq_t)875 * a3;
        a3 = a2;
        a2 = a1;
        a1 = curr;
    }
    return a1;
}
