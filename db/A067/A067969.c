/*
 * A067969  Number of nodes in virtual, "optimal", chordal graphs of diameter
 *          5, degree =n+1.
 * https://oeis.org/A067969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for odd n, let t = (n+1)/2, a(n) = (4/15*t^5 + 2/3*t^4 + 8/3*t^3
 *            + 10/3*t^2 + 46/15*t + 1); for even n, let t = n/2, a(n) =
 *            (4/15*t^5 + 2/3*t^4 + 8/3*t^3 + 10/3*t^2 + 46/15*t + 1) +
 *            (2*(t*(t+1)*(t^2+t+4))/3 + 1); (the fractions result in an
 *            integer value)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 216, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset 1 – not used but harmless */

    seq_t t;
    if (n & 1) {            /* odd n: t = (n+1)/2 */
        t = ((seq_t)n + 1) / 2;
    } else {                /* even n: t = n/2 */
        t = (seq_t)n / 2;
    }

    /* Compute powers of t */
    seq_t t2 = t * t;
    seq_t t3 = t2 * t;
    seq_t t4 = t3 * t;
    seq_t t5 = t4 * t;

    /* Base polynomial numerator (common to both cases) */
    seq_t num = 4 * t5 + 10 * t4 + 40 * t3 + 50 * t2 + 46 * t + 15;
    seq_t part1 = num / 15;      /* P(t) */

    if (n & 1) {                /* odd */
        return part1;
    } else {                    /* even */
        seq_t r = t * (t + 1) * (t2 + t + 4);
        seq_t part2 = (2 * r) / 3 + 1;   /* Q(t) */
        return part1 + part2;
    }
}
