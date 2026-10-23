/*
 * A118837  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 4.
 * https://oeis.org/A118837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   For n > 1, let x = a(n-1), k = digit_len(x).
   p10 = 10^(k-1);
   d   = x / p10;        // first digit
   y   = x % p10;        // remainder when first digit is removed
   a(n) = (y * 10 + d) + 4;
*/
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1, but safe */

    /* static cache – 1-indexed, element 0 unused */
    static seq_t *cache = NULL;
    static idx_t cached = 0;      /* largest index already computed */

    /* allocate or grow cache if needed */
    if (cached < n) {
        /* allocate array of size n+1 (index 0 unused) */
        seq_t *new_buf = (seq_t *)realloc(cache, (n + 1) * sizeof *new_buf);
        if (!new_buf) return 0;  /* allocation failed */
        cache = new_buf;

        /* initialise newly allocated entries to 0 */
        for (idx_t i = cached + 1; i <= n; ++i) cache[i] = 0;

        /* initialise first term if it hasn't been set */
        if (cached < 1) cache[1] = 1;

        /* compute all missing terms up to n */
        idx_t start = (cached < 1) ? 2 : cached + 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            int k = digit_len(prev);                 /* number of digits of prev */
            seq_t p10 = pow10((idx_t)(k - 1));        /* 10^(k-1) */
            seq_t d   = prev / p10;                  /* first digit  */
            seq_t y   = prev % p10;                  /* remainder     */
            cache[i] = (y * 10 + d) + 4;             /* rotate + 4    */
        }
        cached = n;
    }
    return cache[n];
}
