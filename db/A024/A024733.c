/*
 * A024733  a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *          a(1)=a(2)=1 and a(3)=7.
 * https://oeis.org/A024733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+1)/4)} a(2*i-1) * a(n-2*i+1), with
 *            a(1)=1, a(2)=1, a(3)=7
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute the n-th term of OEIS sequence A024733 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* holds values a(1)...a(computed) */
    static idx_t cache_size = 0;         /* allocated length of cache      */
    static idx_t computed = 0;           /* highest n already computed     */

    if (n == 0)                /* not expected, return 0 for safety */
        return 0;

    /* expand cache if necessary */
    if (n > cache_size) {
        idx_t new_size = n * 2;
        if (new_size < n)       /* overflow protection */
            new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;            /* realloc may return NULL – ignore for brevity */
        cache_size = new_size;
    }

    /* build missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val;

        if (k == 1 || k == 2)
            val = (seq_t)1;
        else if (k == 3)
            val = (seq_t)7;
        else {
            seq_t sum = 0;
            idx_t max_i = (k + 1) / 4;   /* floor((k+1)/4) */
            for (idx_t i = 1; i <= max_i; ++i) {
                idx_t j = k - 2 * i + 1;           /* second index */
                seq_t left  = cache[2 * i - 2]; /* a(2*i-1)    */
                seq_t right = cache[j - 1];       /* a(j)         */
                sum += left * right;
            }
            val = sum;
        }

        cache[k - 1] = val;
    }

    computed = n;
    return cache[n - 1];
}
