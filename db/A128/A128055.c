/*
 * A128055  a(n) = 2^A128054(n).
 * https://oeis.org/A128055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n>0, a(n)=a(n-1)*k, where k = 1 if n mod 6 == 1, k =
 *            2 if n mod 6 in {0,2,3,4}, k = 4 if n mod 6 == 5. Thus a(n) =
 *            2^A128054(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t k = i % 6;
        seq_t mult = (k == 5 ? 4 : (k == 1 ? 1 : 2));
        val *= mult;
    }
    return val;
}
