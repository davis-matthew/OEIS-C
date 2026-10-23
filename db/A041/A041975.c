/*
 * A041975  Denominators of continued fraction convergents to sqrt(510).
 * https://oeis.org/A041975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041975 : Denominators of continued fraction convergents to sqrt(510)
   Formula: a(n) = 542*a(n-6) - a(n-12)   (n >= 12)
   Initial terms:
          a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=7, a(5)=12,
          a(6)=535, a(7)=547, a(8)=1082, a(9)=2711, a(10)=3793, a(11)=6504
*/
seq_t a(idx_t n)
{
    /* Allocate a static cache that comfortably holds the indices
       required for the tests (up to about 10 000). */
    enum { CACHE_CAP = 20001 };
    static seq_t cache[CACHE_CAP];
    static idx_t cached = 0;          /* number of values currently computed */

    /* Initialize the cache with the first 12 terms on first use. */
    if (cached == 0) {
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)2;
        cache[3]  = (seq_t)5;
        cache[4]  = (seq_t)7;
        cache[5]  = (seq_t)12;
        cache[6]  = (seq_t)535;
        cache[7]  = (seq_t)547;
        cache[8]  = (seq_t)1082;
        cache[9]  = (seq_t)2711;
        cache[10] = (seq_t)3793;
        cache[11] = (seq_t)6504;
        cached = 12;
    }

    /* Extend the cache up to the requested index if needed. */
    if (n >= cached) {
        /* Note: n is guaranteed to be < CACHE_CAP by the problem limits. */
        for (idx_t i = cached; i <= n; ++i) {
            cache[i] = (seq_t)542 * cache[i - 6] - cache[i - 12];
        }
        cached = n + 1;
    }

    return cache[n];
}
