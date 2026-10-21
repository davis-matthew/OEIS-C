/*
 * A091952  a(1)=1, a(2n)=(a(n)+1) mod 2; a(2n+1)=2*a(2n).
 * https://oeis.org/A091952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1,
 *          a(2n) = (a(n)+1) mod 2,
 *          a(2n+1) = 2*a(2n)
 *
 * The implementation uses simple recursion; the depth is O(log n) so it
 * is efficient for the required ranges.
 */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Even index: n = 2 * m */
    if ((n & (idx_t)1) == 0) {
        seq_t val = a(n >> 1);          /* a(n/2) */
        return (val + (seq_t)1) & (seq_t)1;  /* (a(n/2)+1) mod 2 */
    }

    /* Odd index: n = 2 * m + 1, m = (n-1)/2 */
    seq_t val_even = a(n - 1);          /* this is a(2m) */
    return val_even * (seq_t)2;         /* a(2m+1) = 2 * a(2m) */
}
