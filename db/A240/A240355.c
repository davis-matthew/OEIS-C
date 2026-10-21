/*
 * A240355  Inverse of 72nd cyclotomic polynomial.
 * https://oeis.org/A240355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in (1 + x^12) / (1 + x^36) Hence a(n)
 *            = 0 unless n ≡ 0 or 12 (mod 36); when n = 36k → a(n) = (-1)^k
 *            when n = 12 + 36k → a(n) = (-1)^k
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is only non‑zero when n is a multiple of 12 */
    if ((n % 12ULL) != 0ULL)
        return 0;

    uint64_t m = n / 12ULL;      /* n = 12*m   */

    /* m mod 3 == 2 gives zero */
    if ((m % 3ULL) == 2ULL)
        return 0;

    /* compute k = floor(m/3)  (k is integer so that n = 12*(3k) or 12*(3k+1)) */
    uint64_t k = (m % 3ULL == 0ULL) ? (m / 3ULL) : ((m - 1ULL) / 3ULL);

    /* parity of k determines the sign */
    return (k & 1ULL) ? (seq_t)-1 : (seq_t)1;
}
