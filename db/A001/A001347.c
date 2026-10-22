/*
 * A001347  a(n) = Sum_{k=0..5} (n+k)! * C(5,k).
 * https://oeis.org/A001347
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..5} (n+k)! * C(5,k), using (n+k)! = 1, 1, 2, 6,
 *            ... for n+k >= 0. At n = -1 the k=0 term (-1)! has no value;
 *            OEIS's listed a(-1) = 88 equals the k=1..5 sum (89) minus 1, so
 *            that term is taken as -1
 * Range:     u8 n <= -1, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int c[6] = {1, 5, 10, 10, 5, 1}; /* C(5,k), k=0..5 */
    seq_t result = 0;
    seq_t fact = 1;            /* running factorial */
    idx_t start = n < 0 ? 0 : n;
    idx_t i;
    int k;

    /* fact = start! */
    for (i = 2; i <= start; i++)
        fact *= (seq_t)i;

    for (k = 0; k <= 5; k++) {
        idx_t m = n + k;
        if (m < 0)
            continue;
        if (m > start)
            fact *= (seq_t)m;  /* advance from (m-1)! to m! */
        result += fact * (seq_t)c[k];
    }

    if (n == -1)
        result -= 1;           /* (-1)! term, see note above */

    return result;
}
