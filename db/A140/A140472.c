/*
 * A140472  a(n) = a(n - a(n-1)) + a(floor(n/2)).
 * https://oeis.org/A140472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A140472
 * Recurrence: a(0)=0, a(1)=1, a(2)=2
 * a(n) = a(n - a(n-1)) + a(floor(n/2))   for n ≥ 3
 * Equivalent simple rule:
 *   a(2k) = 2 * a(k)
 *   a(2k-1) = k   (i.e. (n+1)/2 for odd n)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* a(0) */
    if ((n & 1) == 1)               /* odd n: a(2k-1) = k = (n+1)/2 */
        return (seq_t)((n + 1) / 2);
    /* even n: a(2k) = 2 * a(k) */
    return (seq_t)(2 * a(n / 2));
}
