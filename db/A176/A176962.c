/*
 * A176962  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=2, k=-1 and l=1.
 * https://oeis.org/A176962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176962
   Recurrence:  a(0)=1, a(1)=2
   For n ≥ 2
   a(n) = ((7*n-2)*a(n-1) - (11*n-13)*a(n-2) - 3*(n-6)*a(n-3)
          + 12*(n-4)*a(n-4) - 4*(n-5)*a(n-5)) / (n+1)     (derived from the
          given recurrence a(n+1)=∑_{p=0}^n (a(p)*a(n-p)+k)+l with k=‑1, l=1).
*/

#include <stdlib.h>
#include <stdint.h>

/* Cache for memoised terms (indices 0 .. max_computed) */
static seq_t *cache = NULL;
static idx_t cache_size = 0;     /* allocated array size, in terms of count */
static idx_t max_computed = 0;   /* largest index that has been computed */

/* Ensure capacity for at least 'need' terms (indices 0 .. need-1). */
static void ensure_capacity(idx_t need)
{
    if (cache_size < need) {
        idx_t new_size = cache_size ? (cache_size << 1) : 1;
        while (new_size < need) {
            new_size <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Out‑of‑memory – abort. */
            exit(1);
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    /* Initialise cache with the first two terms on the very first call. */
    if (max_computed == 0) {
        ensure_capacity(2);          /* need indices 0 and 1 */
        cache[0] = (seq_t)1;         /* a(0) */
        cache[1] = (seq_t)2;         /* a(1) */
        max_computed = 1;
    }

    /* Compute missing terms up to n. */
    if (n > max_computed) {
        ensure_capacity(n + 1);      /* need indices 0 .. n */

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* Initialise the first six terms explicitly. */
            if (i <= 5) {
                switch (i) {
                    case 0: cache[0] = (seq_t)1; break;
                    case 1: cache[1] = (seq_t)2; break;
                    case 2: cache[2] = (seq_t)3; break;
                    case 3: cache[3] = (seq_t)8; break;
                    case 4: cache[4] = (seq_t)25; break;
                    case 5: cache[5] = (seq_t)87; break;
                }
                continue;
            }

            /* Apply the derived linear recurrence for i ≥ 6. */
            seq_t numerator = cache[i - 1] * (seq_t)(7 * i - 2);
            numerator       += cache[i - 4] * (seq_t)(12 * (i - 4));
            numerator       -= cache[i - 2] * (seq_t)(11 * i - 13);
            numerator       -= cache[i - 3] * (seq_t)(3 * (i - 6));
            numerator       -= cache[i - 5] * (seq_t)(4 * (i - 5));

            cache[i] = numerator / (seq_t)(i + 1);
        }
        max_computed = n;
    }

    return cache[n];
}
