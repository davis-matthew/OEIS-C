/*
 * A176611  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=1, k=1 and l=1.
 * https://oeis.org/A176611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, for n>=1: a(n+1)=∑_{p=0}^{n} (a(p)*a(n-p)+1)+1
 *            equivalently a(n+1)=∑_{p=0}^{n} a(p)*a(n-p)+(n+2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* memoised values */
static idx_t cache_len = 0;          /* number of terms currently stored */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold up to n */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failed */
        cache = tmp;

        /* If this is the first allocation, initialise nothing yet.
           The loop below will fill the required entries. */
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Previous values are known for i <= 5, use the listed constants */
        if (i <= 5) {
            switch ((idx_t)i) {
                case 0: cache[0] = (seq_t)1; break;
                case 1: cache[1] = (seq_t)1; break;
                case 2: cache[2] = (seq_t)5; break;
                case 3: cache[3] = (seq_t)15; break;
                case 4: cache[4] = (seq_t)45; break;
                case 5: cache[5] = (seq_t)151; break;
            }
            continue;
        }

        /* i >= 6: use the 5‑term variable‑coefficient recurrence
           (conjectured to hold for all n >= 6).              */
        long long c1 = -7LL * (long long)i + 2;          /* negative for i>=1 */
        long long c2 =  3LL * (5LL * (long long)i - 7);  /* positive for i>=2 */
        long long c3 = -25LL * (long long)i + 74;        /* negative for i>=4 */
        long long c4 =  24LL * (long long)(i - 4);       /* positive for i>=4 */
        long long c5 =  8LL * (-(long long)i + 5);        /* negative for i>=5 */

        seq_t num = 0;
        num += (seq_t)(-c1) * cache[i - 1];   /* + |c1| * a[i-1]   */
        num -= (seq_t)c2 * cache[i - 2];       /* -  c2  * a[i-2]   */
        num += (seq_t)(-c3) * cache[i - 3];   /* + |c3| * a[i-3]   */
        num -= (seq_t)c4 * cache[i - 4];       /* -  c4  * a[i-4]   */
        num += (seq_t)(-c5) * cache[i - 5];   /* + |c5| * a[i-5]   */

        seq_t denom = (seq_t)(i + 1);
        cache[i] = num / denom;              /* division is exact */
    }

    cache_len = n + 1;
    return cache[n];
}
