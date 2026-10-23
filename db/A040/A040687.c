/*
 * A040687  Continued fraction for sqrt(714).
 * https://oeis.org/A040687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=26; a(n) = period[(n-1) mod 10], where period =
 *            {1,2,1,1,2,1,1,2,1,52}
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)26;
    }
    static const seq_t period[10] = {1, 2, 1, 1, 2, 1, 1, 2, 1, 52};
    return period[(n - 1) % 10];
}
