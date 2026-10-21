/*
 * A050051  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 2 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A050051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=3; for n>=4, a(n)=a(n-1)+a(m) where m = 2*n
 *            - 2 - 2^(p+1) and p is the unique integer with 2^p < n-1 <=
 *            2^(p+1)
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 126, u64 n <= 504,
 *            u128 n <= 3081, big any n
 * Helpers:   floor_log2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_log2.h"

/* Helper: floor(log2(x)) for x>0 */

static seq_t *cache = NULL;      /* array of a values, 0‑based index */
static idx_t  cached_size = 0;   /* number of allocated slots */
static idx_t  computed = 0;      /* largest n for which a(n) is known */

static void ensure_capacity(idx_t n)
{
    if (n <= cached_size) return;
    idx_t new_size = n;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);      /* allocation failed */
    cache = tmp;
    cached_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but return 0 for safety */

    ensure_capacity(n);

    for (idx_t i = computed + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[i-1] = (seq_t)1;
        else if (i == 2)
            cache[i-1] = (seq_t)2;
        else if (i == 3)
            cache[i-1] = (seq_t)3;
        else
        {
            idx_t k = i - 1;                     /* n - 1   */
            idx_t p = floor_log2(k - 1);         /* 2^p < k <= 2^(p+1) */
            idx_t pow = ((idx_t)1 << (p + 1));   /* 2^(p+1) */
            idx_t m = 2 * i - 2 - pow;           /* 2*n - 2 - 2^(p+1) */

            cache[i-1] = cache[i-2] + cache[m-1];
        }
    }
    computed = n;
    return cache[n-1];
}
