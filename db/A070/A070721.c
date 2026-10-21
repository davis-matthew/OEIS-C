/*
 * A070721  Duplicate of A070502.
 * https://oeis.org/A070721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^7 mod 40
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t MOD = 40;
    seq_t x = (seq_t)(n % MOD);   /* base n mod 40 */
    seq_t res = 1;
    for (int i = 0; i < 7; ++i)
    {
        unsigned long long tmp = (unsigned long long)res * (unsigned long long)x;
        res = (seq_t)(tmp % 40ULL);
    }
    return res;
}
