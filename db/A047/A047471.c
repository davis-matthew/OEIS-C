/*
 * A047471  Numbers that are congruent to {1, 3} mod 8.
 * https://oeis.org/A047471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1)-(-1)^n ↦ a(1)=1, a(2)=3, a(3)=9, …
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm1 = (seq_t)n - 1;
    seq_t res = nm1 * 4;          /* 4*(n-1) */

    if (n & 1) {                  /* odd n → (-1)^n = -1 → +1 */
        res += 1;
    } else {                       /* even n → (-1)^n = +1 → -1 */
        res -= 1;
    }
    return res;
}
