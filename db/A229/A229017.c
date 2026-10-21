/*
 * A229017  Number of arrays of median of three adjacent elements of some
 *          length 9 0..n array, with no adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) * +
 *            56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8) (n > 8) * Initial
 *            values: * a(1)=2, a(2)=193, a(3)=2054, a(4)=10735, * a(5)=38768,
 *            a(6)=111311, a(7)=273124, a(8)=597477
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 33, u64 n <= 158, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* just in case; sequence starts at 1 */

    /* Static cache of computed values up to n=10000. */
    static seq_t buf[10001] = {
        (seq_t)2,    (seq_t)193,    (seq_t)2054,
        (seq_t)10735, (seq_t)38768,  (seq_t)111311,
        (seq_t)273124, (seq_t)597477
    };
    static idx_t maxComputed = 8;   /* number of terms already known */

    if (n <= maxComputed) return buf[n-1];

    /* Compute terms up to n, extending the cache. */
    for (idx_t i = maxComputed; i < n; ++i) {
        /* i corresponds to a(i+1), so indices in buf are i-1 .. i-8 */
        buf[i] = (seq_t)8   * buf[i-1]
               - (seq_t)28  * buf[i-2]
               + (seq_t)56  * buf[i-3]
               - (seq_t)70  * buf[i-4]
               + (seq_t)56  * buf[i-5]
               - (seq_t)28  * buf[i-6]
               + (seq_t)8   * buf[i-7]
               - buf[i-8];
    }
    maxComputed = n;
    return buf[n-1];
}
