/*
 * A136712  At step n the sequence lists the number of occurrences of digit (n
 *          mod k), with k>0, in all the numbers from 1 to n. Case k=8.
 * https://oeis.org/A136712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = number of occurrences of digit ((n+1) mod 8) in the
 *            decimal representations of the integers 1, 2, …, n+1
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n + 1;          /* upper bound of the range */
    if (m == 0) return 0;     /* wrap‑around guard for very large n */

    int d = m % 8;            /* digit to count */
    seq_t count = 0;          /* result accumulator */

    for (idx_t i = 1; i <= m; ++i) {
        idx_t x = i;
        while (x) {
            if ((x % 10u) == d) ++count;   /* count the digit */
            x /= 10u;
        }
    }
    return count;
}
