/*
 * A087627  Count ...n,2n,2n...
 * https://oeis.org/A087627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{0<=k<=n : 3 divides k(n-k)} = # * where k ≡ 0 (mod 3)
 *            or k ≡ n (mod 3)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n % 3 is 0, 1 or 2 */
    idx_t r = n % 3;

    /* Count of k in [0,n] with k ≡ 0 (mod 3) */
    seq_t count_div3 = (seq_t)(n / 3) + 1;   /* includes k = 0 */

    /* If n%3 == 0, the second set coincides with the first one,
       so we only count the unique ones. */
    if (r == 0) {
        return count_div3;
    }

    /* Count of k in [0,n] with k ≡ r (mod 3) */
    seq_t count_congr = (seq_t)((n - r) / 3) + 1;
    return count_div3 + count_congr;
}
