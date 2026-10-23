/*
 * A144429  Starts 1 2 3 then successive terms have differences 1 2 3.
 * https://oeis.org/A144429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=3, for n ≥ 4 a(n) = a(n‑1) + ( (n % 3 == 0)
 *            ? 3 : (n % 3) ) Equivalent closed form (O(1) time): if n % 3 ==
 *            0 : a(n) = 6 * (n / 3) - 3 if n % 3 == 1 : a(n) = 2 * n - 4 //
 *            for n ≥ 4 if n % 3 == 2 : a(n) = 6 * ((n - 2) / 3) // for n ≥ 5
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) return (seq_t)n;          /* a(1)=1, a(2)=2, a(3)=3 */

    idx_t mod = n % 3;
    if (mod == 0) {
        idx_t k = n / 3;
        return (seq_t)6 * (seq_t)k - (seq_t)3;
    } else if (mod == 1) {
        return (seq_t)2 * (seq_t)n - (seq_t)4;
    } else { /* mod == 2 */
        return (seq_t)6 * (seq_t)((n - 2) / 3);
    }
}
