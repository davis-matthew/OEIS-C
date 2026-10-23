/*
 * A015802  Inverse of 1793rd cyclotomic polynomial.
 * https://oeis.org/A015802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power‑series * 1/Φ_1793(x)
 *            = (1−x)^(−1)·(1−x^11)·(1−x^163)·(1−x^1793)^(−1), * which reduces
 *            modulo x^1793 to a periodic sequence with * period 1793. The
 *            resulting table is * a(n)=1 for n≡0,…,10 (mod 1793) * a(n)=0 for
 *            n≡11,…,162 (mod 1793) * a(n)=−1 for n≡163...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t PERIOD = 1793ULL;
    idx_t m = n % PERIOD;
    if (m <= 10ULL)          return 1;
    if (m <= 162ULL)         return 0;
    if (m <= 173ULL)         return -1;
    return 0;
}
