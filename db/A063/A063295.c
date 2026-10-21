/*
 * A063295  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          22 ).
 * https://oeis.org/A063295
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A063295 – Dimension of the space of weight n cuspidal newforms for Γ₁(22)
   Recurrence:  a(n) = a(n‑4) + a(n‑6) – a(n‑10)     for n > 13
   Initial values:
        a(2)  = -1
        a(3)  = 4
        a(4)  = 10
        a(5)  = 15
        a(6)  = 20
        a(7)  = 25
        a(8)  = 30
        a(9)  = 33
        a(10) = 40
        a(11) = 43
        a(12) = 50
        a(13) = 55
*/

#include <stdlib.h>

/* static cache for memoised values */
static seq_t *cache = NULL;          /* array a[0 … cache_len-1]  */
static idx_t cache_len = 0;          /* number of valid entries */

/* extend cache to include a[n] for n >= 0 */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;       /* already computed */

    idx_t old_len = cache_len;
    idx_t new_len = n + 1;           /* we want indices 0 … n */

    /* realloc must be cast for C++ compatibility */
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) abort();        /* out of memory */
    cache = tmp;

    /* initialise or compute new entries */
    for (idx_t i = old_len; i <= n; ++i)
    {
        /* undefined for i < 2, always return 0 */
        if (i < 2) {
            cache[i] = (seq_t)0;
        }
        else if (i == 2) cache[i] = (seq_t)(-1);
        else if (i == 3) cache[i] = (seq_t)4;
        else if (i == 4) cache[i] = (seq_t)10;
        else if (i == 5) cache[i] = (seq_t)15;
        else if (i == 6) cache[i] = (seq_t)20;
        else if (i == 7) cache[i] = (seq_t)25;
        else if (i == 8) cache[i] = (seq_t)30;
        else if (i == 9) cache[i] = (seq_t)33;
        else if (i == 10) cache[i] = (seq_t)40;
        else if (i == 11) cache[i] = (seq_t)43;
        else if (i == 12) cache[i] = (seq_t)50;
        else if (i == 13) cache[i] = (seq_t)55;
        else /* i > 13 */ 
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined for offset < 2 */
    ensure_cache(n);
    return cache[n];
}
