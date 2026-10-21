/*
 * A069218  a(1)=1, a(2)=8; for n >= 1, a(n+2)=(a(n+1)+a(n))/3 if
 *          (a(n+1)+a(n)==0 (mod 3)); a(n+2)=(a(n+1)+a(n))/2 if
 *          (a(n+1)+a(n)==0 (mod 2)); a(n+2)=a(n+1)+a(n) otherwise.
 * https://oeis.org/A069218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=8;
   For n ≥ 1,
       a(n+2) = (a(n+1)+a(n))/3   if (a(n+1)+a(n)) ≡ 0 (mod 3)
                (a(n+1)+a(n))/2   if (a(n+1)+a(n)) ≡ 0 (mod 2)
                a(n+1)+a(n)        otherwise                     */
#include <stdlib.h>

static seq_t *cache = NULL;     /* storage for computed terms, 1‑based */
static idx_t  computed = 0;     /* maximum index that has been computed */

/* Ensure the cache has room for indices up to 'need' (inclusive) */
static void grow_cache(idx_t need)
{
    if (cache == NULL) {
        size_t sz = (need >= 2 ? need + 1 : 3);
        cache = (seq_t *)malloc(sizeof(seq_t) * sz);
        /* initialise base terms once */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        computed = 2;
    } else if (need + 1 > computed) { /* we need a larger buffer */
        size_t sz = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * sz);
        if (tmp != NULL) {
            cache = tmp;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined, but return 0 */

    grow_cache(n);

    if (n <= computed) return cache[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        if (sum % (seq_t)3 == 0) {
            cache[i] = sum / (seq_t)3;
        } else if (sum % (seq_t)2 == 0) {
            cache[i] = sum / (seq_t)2;
        } else {
            cache[i] = sum;
        }
    }
    computed = n;
    return cache[n];
}
