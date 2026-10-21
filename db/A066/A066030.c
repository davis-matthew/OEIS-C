/*
 * A066030  Card{i<=n, floor(n/i)=0 mod(i)}.
 * https://oeis.org/A066030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { 1 ≤ i ≤ n | n mod i² < i }
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The condition "floor(n/i) = 0 mod i" is equivalent to (n mod i²) < i. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t sq = i * i;          /* i²  (safe because i ≤ n and n is small for the tests) */
        idx_t r  = n % sq;         /* n mod i²  */
        if (r < i)
            ++count;
    }
    return count;
}
