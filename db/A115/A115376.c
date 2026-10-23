/*
 * A115376  <h[d+1,d-1],s[d,d]*s[d,d]*s[d,d]> where h[d+1,d-1] is a
 *          homogeneous symmetric function, s[d,d] is a Schur function indexed
 *          by two parts, * represents the Kronecker product and <, > is the
 *          standard scalar product on symmetric functions.
 * https://oeis.org/A115376
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 473, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = a(n-1)+4*a(n-2)-3*a(n-3)-7*a(n-4)+2*a(n-5)+8*a(n-6)+2*a(n-7)
          -7*a(n-8)-3*a(n-9)+4*a(n-10)+a(n-11)-a(n-12)
   for n>13 with a(2)=1,a(3)=1,a(4)=5,a(5)=6,a(6)=16,a(7)=20,a(8)=41,
   a(9)=51,a(10)=90,a(11)=111,a(12)=177,a(13)=216
*/

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment.          */
/* They are unsigned integer types (built‑in or arbitrary–precision). */

static seq_t *cache = NULL;      /* allocated array of sequence values     */
static size_t cache_alloc = 0;   /* number of elements allocated          */
static size_t cache_max   = 0;   /* highest index that has been computed   */

seq_t a(idx_t n) {
    /* Offset: a(2) is the first term.  Return 0 for indices < 2. */
    if (n < 2)
        return (seq_t)0;

    /* Ensure the array is large enough to hold index n. */
    if (cache_alloc <= n) {
        size_t new_alloc = (size_t)(n + 1);          /* n is unsigned 64‑bit */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_alloc * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failed   */
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Fill base values a(2)..a(13) if not yet computed. */
    if (cache_max < 13) {
        static const seq_t base[14] = {
            /* a(0)..a(13) – indices 0 and 1 are unused (set to 0). */
            0, 0, 1, 1, 5, 6, 16, 20, 41, 51, 90, 111, 177, 216
        };
        size_t i;
        size_t start = (cache_max < 2) ? 2 : cache_max + 1;
        if (start <= 13) {
            for (i = start; i <= 13 && i <= n; ++i) {
                cache[i] = base[i];
            }
        }
        if (i > cache_max)
            cache_max = i - 1;   /* update max after filling base terms   */
    }

    /* Compute values for indices > max(cache_max,13) up to n. */
    if (n > cache_max) {
        size_t i;
        /* If we have less than 14 computed, start from 14. */
        size_t start = (cache_max < 13) ? 14 : cache_max + 1;
        for (i = start; i <= n; ++i) {
            seq_t val = 0;
            val += cache[i-1];
            val += 4 * cache[i-2];
            val -= 3 * cache[i-3];
            val -= 7 * cache[i-4];
            val += 2 * cache[i-5];
            val += 8 * cache[i-6];
            val += 2 * cache[i-7];
            val -= 7 * cache[i-8];
            val -= 3 * cache[i-9];
            val += 4 * cache[i-10];
            val += cache[i-11];
            val -= cache[i-12];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
