/*
 * A130481  a(n) = Sum_{k=0..n} (k mod 3) (i.e., partial sums of A010872).
 * https://oeis.org/A130481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if (n+1) mod 3 ≠ 0 * a(n) = n + 1 if (n+1) mod 3 = 0 *
 *            * This uses the observation that the partial sums of k mod 3
 *            increase by * one only when n+1 is a multiple of 3. The
 *            implementation is O(1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod = (n + 1) % 3;   /* compute (n+1) mod 3 using the index type */
    seq_t res = (seq_t)n;      /* start from n (converted to seq_t) */
    if (mod == 0)              /* add 1 only when n+1 is divisible by 3 */
        res += 1;
    return res;
}
