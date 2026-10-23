/*
 * A041851  Denominators of continued fraction convergents to sqrt(447).
 * https://oeis.org/A041851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 296*a(n-2) - a(n-4), a(0)=1, a(1)=7, a(2)=295, a(3)=2072
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base terms */
    static const seq_t base[4] = {
        (seq_t)1,
        (seq_t)7,
        (seq_t)295,
        (seq_t)2072
    };

    if (n < 4)
        return base[n];

    /* Sliding window: indices i-4, i-3, i-2, i-1 */
    seq_t prev4 = base[0];
    seq_t prev3 = base[1];
    seq_t prev2 = base[2];
    seq_t prev1 = base[3];

    for (idx_t i = 4; i <= n; ++i) {
        seq_t curr = ((seq_t)296) * prev2 - prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
