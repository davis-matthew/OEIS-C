/*
 * A121528  a(1)=1. a(n) = n-th integer from among those positive integers
 *          which are coprime to the sum of the previous terms of this
 *          sequence.
 * https://oeis.org/A121528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121528:
 *   a(1) = 1
 *   a(n) = the n-th positive integer that is coprime to
 *          the sum of a(1)..a(n‑1)
 * This implementation builds the sequence lazily and caches all
 * previously computed terms.  It works for any integer type supplied
 * by the build system (uint8_t, uint16_t, uint32_t, uint64_t,
 * unsigned __int128, or an arbitrary‑precision integer class).
 *
 * The cache is maintained in a static array that grows by
 * doubling its capacity when required.  Copying of seq_t values
 * is performed using element‑wise assignment to avoid
 * undefined behaviour when seq_t is a class type.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* array of computed terms       */
static size_t cap   = 0;      /* current capacity of cache     */
static size_t len   = 0;      /* number of terms stored so far */
static seq_t sum   = 0;       /* running sum of stored terms   */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* unused, but defined */

    /* Ensure the cache can hold at least n terms */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap * 2 : n;
        if (newcap < (size_t)n) newcap = n;

#ifdef __cplusplus
        seq_t *tmp = new seq_t[newcap];
        for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
#endif
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)len; i < n; ++i) {
        seq_t S = sum;            /* sum of previous terms   */
        seq_t next;

        if (S == (seq_t)0) {      /* gcd(1,0) == 1, so the only coprime is 1 */
            next = (seq_t)1;
        } else {
            seq_t cand = 1;
            idx_t  cnt = 0;
            while (1) {
                if (gcd(cand, S) == (seq_t)1) {
                    ++cnt;
                    if (cnt == (idx_t)(i + 1)) {
                        next = cand;
                        break;
                    }
                }
                ++cand;
            }
        }

        cache[i] = next;
        sum += next;
    }

    len = n;
    return cache[n - 1];
}
