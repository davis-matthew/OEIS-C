/*
 * A102690  Number of n-expodigital numbers (i.e., numbers m such that m^n has
 *          exactly n decimal digits).
 * https://oeis.org/A102690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = number of decimal integers m (0–9 for n=1, 1–9 for n>1)
 *            such that m^n has exactly n decimal digits. For n > 1 this is
 *            equivalent to finding the smallest m∈[1,9] with
 *            floor(n·log10(m)) = n−1; then a(n) = 10−m. For n = 1 the answer
 *            is 10 (all digits 0–9)
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)10;        /* 0–9 all have 1 digit */

    /* Common logarithms of 1–9, high‑precision constants. */
    static const double LOG10[10] = {
        0.0,
        0.0,
        0.3010299956639812,
        0.47712125471966244,
        0.6020599913279624,
        0.6989700043360188,
        0.7781512503836436,
        0.8450980400142568,
        0.9030899869919435,
        0.9542425094393249
    };

    const double EPS = 1e-12;   /* safety margin for rounding */
    idx_t m_min = 10;           /* sentinel for "no m found" */

    for (idx_t m = 1; m <= 9; ++m) {
        double val = (double)n * LOG10[m];
        idx_t digits_minus_one = (idx_t)(val + EPS);   /* floor(n·log10(m)) */
        if (digits_minus_one == n - 1) {
            m_min = m;
            break;
        }
    }

    if (m_min == 10)          /* no m satisfies the condition */
        return (seq_t)0;

    return (seq_t)(10 - m_min);
}
