/*
 * A049206  Maximum mean distance between cards during perfect faro shuffles,
 *          with cut, to return to original order in A024222.
 * https://oeis.org/A049206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n+1)/2) – (n mod 4 == 3 ? 1 : 0) (a(1)=0)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t val = ((seq_t)n + 1) / 2;          /* floor((n+1)/2) */
    if ((n % 4) == 3) val -= 1;              /* subtract 1 when n ≡ 3 (mod 4) */
    return val;
}
