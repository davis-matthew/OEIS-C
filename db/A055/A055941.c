/*
 * A055941  a(n) = Sum_{j=0..k-1} (i(j) - j) where n = Sum_{j=0..k-1} 2^i(j).
 * https://oeis.org/A055941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of zeros to the right of each 1‐bit in the binary
 *            representation of n Example: n = 20 = 10100₂ → zeros to right of
 *            low 1 = 2, of high 1 = 3 → a(20)=5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t  result = 0;
    idx_t  zero_cnt = 0;
    idx_t  temp = n;

    while (temp)
    {
        if (temp & 1ull)           /* current bit is 1 */
            result += (seq_t)zero_cnt;
        else
            zero_cnt++;            /* current bit is 0 */
        temp >>= 1;
    }

    return result;
}
