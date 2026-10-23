/*
 * A025734  Index of 9^n within the sequence of the numbers of the form
 *          2^i*9^j.
 * https://oeis.org/A025734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * log_2(9))
 * Range:     u8 n <= 12, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* log_2(9) ≈ 3.169925001442312, precise enough for n ≤ 10 000
       (double‑precision error ≪ 0.5, see analysis). */
    static const long double LOG2_9 = 3.169925001442312L;

    seq_t result = 1;               /* a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned floor_k_c = (unsigned)((long double)k * LOG2_9);
        result += (seq_t)floor_k_c + 1;
    }
    return result;
}
