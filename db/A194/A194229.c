/*
 * A194229  Partial sums of A057357.
 * https://oeis.org/A194229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d∈{2,4,6}} [ (t_d+1)*(n-d+1) – 7*t_d*(t_d+1)/2 ],
 *            where t_d = ⌊(n-d)/7⌋ and the summand is taken only if n ≥ d.
 *            This yields the partial sums of A057357
 * Range:     u8 n <= 34, u16 n <= 552, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    const int offsets[3] = {2, 4, 6};

    for (int i = 0; i < 3; ++i) {
        idx_t d = (idx_t)offsets[i];
        if (n < d) continue;                      /* no terms for this offset */

        idx_t t = (n - d) / 7;                     /* floor((n-d)/7) */

        seq_t tn   = (seq_t)t;
        seq_t nd1  = (seq_t)(n - d + 1);

        seq_t term1 = (tn + (seq_t)1) * nd1;
        seq_t term2 = (seq_t)7 * tn * (tn + (seq_t)1) / (seq_t)2;

        res += term1 - term2;
    }
    return res;
}
