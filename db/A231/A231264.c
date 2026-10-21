/*
 * A231264  Number of (2+1) X (n+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A231264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -2*a(n-2) -8*a(n-3) +11*a(n-4) -4*a(n-6)
 *            -4*a(n-7) -4*a(n-8) +8*a(n-9) for n >= 10 Initial terms: a(1)=6,
 *            a(2)=15, a(3)=32, a(4)=83, a(5)=211, a(6)=557, a(7)=1471,
 *            a(8)=3909, a(9)=10387. The implementation stores the values in a
 *            static cache and grows it as needed
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Internal cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cap = 0;     /* current allocated size (highest index + 1)   */
    static idx_t built = 0;    /* highest index for which cache[*] is valid   */

    if (n < 1)   /* undefined for n=0; return 0 for safety */
        return (seq_t)0;

    /* --- allocation / growth --------------------------------- */
    if (cache == NULL) {                 /* first call: allocate minimum cache */
        idx_t init_cap = n > 10 ? n : 10;
        cache = (seq_t *)malloc((init_cap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failure */
        cap = init_cap;
        /* initial terms 1 … 9 (index 0 unused) */
        cache[1] = 6;
        cache[2] = 15;
        cache[3] = 32;
        cache[4] = 83;
        cache[5] = 211;
        cache[6] = 557;
        cache[7] = 1471;
        cache[8] = 3909;
        cache[9] = 10387;
        built = 9;
    } else if (n > cap) {               /* enlarge cache if necessary */
        idx_t new_cap = n > cap * 2 ? n : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return cache[built]; /* keep old values on failure */
        cache = tmp;
        cap = new_cap;
    }

    /* --- compute missing terms -------------------------------- */
    if (built < n) {
        for (idx_t i = built + 1; i <= n; ++i) {
            /* compute using the recurrence; split into positive
               and negative contributions to avoid unsigned underflow */
            seq_t pos = 4 * cache[i - 1] + 11 * cache[i - 4] + 8 * cache[i - 9];
            seq_t neg = 2 * cache[i - 2] + 8 * cache[i - 3]
                      + 4 * cache[i - 6] + 4 * cache[i - 7]
                      + 4 * cache[i - 8];
            cache[i] = pos - neg;
        }
        built = n;
    }

    return cache[n];
}
