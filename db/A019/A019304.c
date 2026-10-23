/*
 * A019304  "Pascal sweep" for k=3: draw a horizontal line through the 1 at
 *          C(k,0) in Pascal's triangle; rotate this line and record the sum
 *          of the numbers on it (excluding the initial 1).
 * https://oeis.org/A019304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even a(n) = (n + 7) / 2 if n ≡ 1 (mod 4) a(n) =
 *            12 + 6*((n - 3)/4) + ((n - 3)/4) * (((n - 3)/4) - 1) / 2 if n ≡
 *            3 (mod 4) (Integer division is used throughout.)
 * Range:     u8 n <= 74, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even indices give the value 1 */
    if ((n & 1) == 0)
        return 1;

    /* odd indices */
    if ((n & 3) == 1)          /* n ≡ 1 (mod 4) */
    {
        /* (n + 7) is always even for n ≡ 1 (mod 4) */
        return ((seq_t)n + 7) / 2;
    }
    else                       /* n ≡ 3 (mod 4) */
    {
        /* compute m = (n - 3)/4  (integer division) */
        idx_t m = (n - 3) / 4;
        seq_t mm = (seq_t)m;

        /* a = 12 + 6*m + m*(m-1)/2 */
        seq_t res = 12;
        res += 6 * mm;
        res += (mm * (mm - 1)) / 2;
        return res;
    }
}
