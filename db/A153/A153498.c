/*
 * A153498  Palindromes formed from concatenation of A147759(n) and the same
 *          string A147759(n) but without its initial digit.
 * https://oeis.org/A153498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 101*a(n-1) - 1110*a(n-2) + 102010*a(n-3) - 111000*a(n-4)
 *            + 1010000*a(n-5) - 1000000*a(n-6)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t sz = 0;

    if (n == 0) return 0; /* undefined for n < 1, return 0 */

    /* if n already cached, return it */
    if (n <= sz) return cache[n - 1];

    /* enlarge cache to hold terms up to n */
    idx_t new_sz = n;
    cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!cache) return 0; /* allocation failed */

    for (idx_t i = sz + 1; i <= new_sz; ++i) {
        seq_t val;
        if (i == 1)        val = (seq_t)1;
        else if (i == 2)   val = (seq_t)111;
        else if (i == 3)   val = (seq_t)10101;
        else if (i == 4)   val = (seq_t)1001001;
        else if (i == 5)   val = (seq_t)101010101;
        else if (i == 6)   val = (seq_t)10110101101;
        else {
            /* recurrence for i >= 7 */
            seq_t a1 = cache[i - 2]; /* a(i-1) */
            seq_t a2 = cache[i - 3]; /* a(i-2) */
            seq_t a3 = cache[i - 4]; /* a(i-3) */
            seq_t a4 = cache[i - 5]; /* a(i-4) */
            seq_t a5 = cache[i - 6]; /* a(i-5) */
            seq_t a6 = cache[i - 7]; /* a(i-6) */

            seq_t pos = (seq_t)101 * a1 + (seq_t)102010 * a3 + (seq_t)1010000 * a5;
            seq_t neg = (seq_t)1110 * a2 + (seq_t)111000 * a4 + (seq_t)1000000 * a6;
            val = pos - neg; /* unsigned subtraction is safe: result is guaranteed positive */
        }
        cache[i - 1] = val;
    }

    sz = new_sz;
    return cache[n - 1];
}
