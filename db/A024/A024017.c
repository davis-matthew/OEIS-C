/*
 * A024017  a(n) = 2^n - n^7.
 * https://oeis.org/A024017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^7 Computation: for n <= 9 use direct formula with
 *            64‑bit arithmetic for n >= 10 use the linear recurrence a(n) =
 *            10*a(n-1)-44*a(n-2)+112*a(n-3)-182*a(n-4)+196*a(n-5)-140*a(n-6)+64*a(n-7)-17*a(n-8)+2*a(n-9)
 *            (a(0) = 1, a(1) = 1, ..., a(9) = -4782457)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 21, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached entries (cache_len == cache size) */

    /* enlarge cache if necessary */
    if (cache_len <= n) {
        /* allocate new cache of size n+1 */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return 0 (undefined for this case) */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 9) {
                /* direct computation using 64‑bit arithmetic – safe because i ≤ 9 */
                long long pow_n7 = 1;
                for (int k = 0; k < 7; ++k)
                    pow_n7 *= (long long)i;
                long long val = ((long long)1 << i) - pow_n7;
                cache[i] = (seq_t)val;
            } else {
                /* recurrence for i ≥ 10, indexes i-1 … i-9 are already valid */
                seq_t sum = 0;
                sum += (seq_t)10   * cache[i - 1];
                sum -= (seq_t)44   * cache[i - 2];
                sum += (seq_t)112  * cache[i - 3];
                sum -= (seq_t)182  * cache[i - 4];
                sum += (seq_t)196  * cache[i - 5];
                sum -= (seq_t)140  * cache[i - 6];
                sum += (seq_t)64   * cache[i - 7];
                sum -= (seq_t)17   * cache[i - 8];
                sum += (seq_t)2    * cache[i - 9];
                cache[i] = sum;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
