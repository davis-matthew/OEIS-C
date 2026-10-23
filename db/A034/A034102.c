/*
 * A034102  Fractional part of square root of a(n) starts with digit 6.
 * https://oeis.org/A034102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer m such that the fractional part of sqrt(m)
   starts with the digit 6, i.e. 0.6 ≤ fract(sqrt(m)) < 0.7.  */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t low_num  = ksq * 100ULL + 120ULL * (seq_t)k + 36ULL;
        seq_t low      = (low_num + 99ULL) / 100ULL;     /* ceil */
        seq_t high_num = ksq * 100ULL + 140ULL * (seq_t)k + 49ULL;
        seq_t high     = high_num / 100ULL;              /* floor */
        if (low <= high) {
            for (seq_t m = low; m <= high; ++m) {
                ++count;
                if (count == n) return m;
            }
        }
    }
}
