/*
 * A221993  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..2 array extended with zeros and
 *          convolved with 1,4,6,4,1.
 * https://oeis.org/A221993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) – 2*a(n-4) – 3*a(n-6) + 2*a(n-7) +
 *            a(n-9) Initial values: a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=10,
 *            a(6)=15, a(7)=22, a(8)=36, a(9)=56
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 50, u64 n <= 101,
 *            u128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */

seq_t a(idx_t n)
{
    /* 1‑based cache; cache[0] is unused */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* number of elements allocated (index 0..cap-1) */
    static idx_t  max_n   = 0;   /* largest n whose value is cached */

    /* Ensure there is space for index n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? cap : 10;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();                      /* out‑of‑memory */
        cache = tmp;
        cap   = new_cap + 1;                    /* indices 0 … new_cap */
    }

    /* Initialise the first nine terms on first use */
    if (max_n == 0) {
        cache[1] = 2;
        cache[2] = 3;
        cache[3] = 4;
        cache[4] = 6;
        cache[5] = 10;
        cache[6] = 15;
        cache[7] = 22;
        cache[8] = 36;
        cache[9] = 56;
        max_n   = 9;
    }

    /* Return a cached value if already computed */
    if (n <= max_n) return cache[n];

    /* Compute terms up to n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += cache[i - 3] * 3;
        val -= cache[i - 4] * 2;
        val -= cache[i - 6] * 3;
        val += cache[i - 7] * 2;
        val += cache[i - 9];
        cache[i] = val;
    }
    max_n = n;
    return cache[n];
}
