/*
 * A207540  Degrees (with multiplicity) of simple surface singularities (ADE
 *          singularities, Du Val singularities, double rational points,
 *          Gorenstein quotient singularities, Klein singularities).
 * https://oeis.org/A207540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th element of the sorted multiset of even degrees
 *            produced by the generating formulas: 4k+2 (k ≥ 1), 2k+2 (k ≥ 0),
 *            2n-2 (n ≥ 4) and the special degrees {12, 18, 30}. Each degree
 *            is repeated for every formula that yields it
 * Range:     u8 n <= 38, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, guard for n==0 */
    idx_t d     = 2;               /* start at the smallest even degree */
    idx_t cum   = 0;               /* cumulative count of terms seen so far */

    while (1) {
        idx_t m = 0;
        /* multiplicity from 2k+2 (k ≥ 0) – all even numbers ≥ 2 */
        m++;

        /* multiplicity from 4k+2 (k ≥ 1) – requires d ≥ 6 and d % 4 == 2 */
        if (d >= 6 && (d & 3) == 2)
            m++;

        /* multiplicity from 2n-2 (n ≥ 4) – requires d ≥ 6 */
        if (d >= 6)
            m++;

        /* special extra multiplicities for the sporadic cases */
        if (d == 12 || d == 18 || d == 30)
            m++;

        cum += m;
        if (cum >= n)
            return (seq_t)d;
        d += 2;                      /* next even degree */
    }
}
