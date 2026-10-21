/*
 * A227013  a(n) = floor(M(g(n-1)+1,..,g(n))), where M is the harmonic mean
 *          and g(n) = n^4.
 * https://oeis.org/A227013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(2) = 6; For n ≥ 3, let m = n – 2. Then a(n) = (47 +
 *            126*m + 123*m² + 54*m³ + 9*m⁴ + (m % 3 == 0 ? -2 : 1)) / 9 (the
 *            numerator is always divisible by 9, giving an integer)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 6;

    idx_t m = n - 2;
    seq_t ms  = (seq_t)m;
    seq_t m2  = ms * ms;
    seq_t m3  = m2 * ms;
    seq_t m4  = m3 * ms;

    seq_t num = (seq_t)47
              + (seq_t)126 * ms
              + (seq_t)123 * m2
              + (seq_t)54  * m3
              + (seq_t)9   * m4;

    if (m % 3 == 0)
        num -= 2;
    else
        num += 1;

    return num / 9;   /* integer division; division is exact */
}
