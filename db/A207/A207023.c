/*
 * A207023  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25 * ( C(n+4,5) + 19*C(n+3,5) - 14*C(n+2,5) + 7*C(n+1,5)
 *            - 2*C(n,5) )
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 69, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom5(idx_t m)
{
    if (m < 5) return 0;
    seq_t res = 1;
    /* Compute (m-4)*(m-3)*(m-2)*(m-1)*m / 120 using iterative division. */
    for (idx_t k = 1; k <= 5; ++k) {
        seq_t factor = (seq_t)(m - 5 + k);   /* factor = m-5+k = m-(5-k) */
        res = (res * factor) / k;           /* exact division each step */
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t c1 = binom5(n + 4);
    seq_t c2 = binom5(n + 3);
    seq_t c3 = binom5(n + 2);
    seq_t c4 = binom5(n + 1);
    seq_t c5 = binom5(n);

    seq_t pos = c1 + 19 * c2 + 7 * c4;     /* sum of positive terms */
    seq_t neg = 14 * c3 + 2 * c5;          /* sum of negative terms */
    seq_t sum = pos - neg;                 /* guaranteed non‑negative */

    return 25 * sum;
}
