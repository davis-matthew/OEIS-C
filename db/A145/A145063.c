/*
 * A145063  Hankel transform of A145062.
 * https://oeis.org/A145063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = if (n % 2 == 0) C(n/2+2,2)*(-1)^(n/2) else (-1)^((n+1)/2)
 * Range:     i8 n <= 29, i16 n <= 509, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1ULL) /* n is odd */
    {
        idx_t exp = (n + 1) >> 1;          /* (n+1)/2 */
        seq_t sign = (exp & 1ULL) ? (seq_t)-1 : (seq_t)1;
        return sign;
    }
    else /* n is even */
    {
        idx_t k = n >> 1;                  /* n/2 */
        seq_t bin = ((seq_t)k + 2) * ((seq_t)k + 1) / 2; /* C(k+2,2) */
        seq_t sign = (k & 1ULL) ? (seq_t)-1 : (seq_t)1;
        return bin * sign;
    }
}
