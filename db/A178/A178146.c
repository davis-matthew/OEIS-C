/*
 * A178146  a(n) is the number of distinct prime factors <= 5 of n.
 * https://oeis.org/A178146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [2|n] + [3|n] + [5|n] (count of distinct primes ≤5
 *            dividing n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    if ((n & 1ULL) == 0)   /* divisible by 2 */
        ans++;
    if (n % 3ULL == 0)      /* divisible by 3 */
        ans++;
    if (n % 5ULL == 0)      /* divisible by 5 */
        ans++;
    return ans;
}
