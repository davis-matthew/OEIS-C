/*
 * A025713  Index of 6^n within the sequence of the numbers of the form
 *          3^i*6^j.
 * https://oeis.org/A025713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} (⌊k·log₃(6)⌋ + 1) (index of 6ⁿ in the
 *            sorted list of numbers of the form 3ⁱ·6ʲ)
 * Range:     u8 n <= 16, u16 n <= 282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* log₃(6) ≈ 1.63092975357145720380  (high‑precision constant) */
    static const long double LOG3_6 = 1.63092975357145720380L;

    /* a(0) = 1 */
    seq_t result = (seq_t)1;

    /* Compute successive increments: floor(k·log₃(6)) + 1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* floor(k·LOG3_6) – truncation of positive long double */
        unsigned long long floor_val = (unsigned long long)(k * LOG3_6 + 1e-12L);
        result += (seq_t)(floor_val + 1);
    }

    return result;
}
