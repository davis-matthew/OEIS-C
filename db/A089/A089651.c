/*
 * A089651  Partial sums of the sequence : a(1)=1, a(1), a(1), a(1), a(1),
 *          a(2), a(2), a(2), a(2), a(3), a(3), a(3), a(3), a(4), ... each
 *          term (not a(1)) repeated 4 times.
 * https://oeis.org/A089651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(floor((n+2)/4)), a(1) = 1
 * Range:     u8 n <= 39, u16 n <= 254, u32 n <= 3355, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) { /* offset 1, but guard against zero */
        return (seq_t)0;
    }

    static seq_t *cache = NULL;
    static idx_t cachecap = 0;  /* maximum index that the array can store */
    static idx_t cachelen = 0;  /* highest index already computed */

    /* Ensure the array has room for indices up to n */
    if (cachecap < n) {
        idx_t newcap = cachecap ? cachecap << 1 : 64;
        if (newcap < n) newcap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory – unlikely */
        cache = tmp;
        cachecap = newcap;
    }

    /* Compute missing terms up to n */
    if (cachelen < n) {
        if (cachelen == 0) {               /* initialize base */
            cache[1] = (seq_t)1;
            cachelen = 1;
        }
        for (idx_t i = cachelen + 1; i <= n; ++i) {
            /* a(i) = a(i-1) + a(floor((i+2)/4)) */
            cache[i] = cache[i - 1] + cache[(i + 2) / 4];
        }
        cachelen = n;
    }

    return cache[n];
}
