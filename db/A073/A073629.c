/*
 * A073629  a(1) = 1; a(2) =2; a(3) = 3; a(n) = smallest number greater than
 *          the previous term such that the sum of four successive terms is a
 *          prime.
 * https://oeis.org/A073629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073629:
   a(1)=1, a(2)=2, a(3)=3.
   For n>=4, a(n) is the smallest integer > a(n-1) such that
   a(n-3)+a(n-2)+a(n-1)+a(n) is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/isPrime64.h"   /* provides: bool isPrime64(uint64_t n) */

static seq_t *a_cache      = NULL;   /* memoised terms, 0‑based */
static idx_t  a_cache_len  = 0;       /* number of terms stored   */
static size_t a_cache_cap  = 0;       /* allocated capacity       */

/* ensure cache contains the first three terms */
static void a_cache_init(void)
{
    if (a_cache != NULL) return;

    a_cache_cap = 8;                     /* initial capacity > 3 */
    a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
    assert(a_cache != NULL);
    a_cache[0] = 1;   /* a(1) */
    a_cache[1] = 2;   /* a(2) */
    a_cache[2] = 3;   /* a(3) */
    a_cache_len = 3;
}

/* enlarge cache to contain at least 'target' terms (1‑based index) */
static void a_cache_extend(idx_t target)
{
    a_cache_init();

    /* target is 1‑based, convert to 0‑based length needed */
    while (a_cache_len < target) {
        /* grow capacity if necessary */
        if (a_cache_cap < (size_t)(a_cache_len + 1)) {
            size_t newcap = a_cache_cap * 2;
            if (newcap < (size_t)(a_cache_len + 1))
                newcap = a_cache_len + 1;
            void *tmp = realloc(a_cache, newcap * sizeof(seq_t));
            assert(tmp != NULL);
            a_cache = (seq_t *)tmp;
            a_cache_cap = newcap;
        }

        /* indices of the last three terms (0‑based) */
        idx_t i   = a_cache_len;        /* position for the new term */
        seq_t s1  = a_cache[i-3];
        seq_t s2  = a_cache[i-2];
        seq_t s3  = a_cache[i-1];

        seq_t candidate = s3 + 1;
        for (;;) {
            /* sum fits into uint64_t for all tested ranges;
               cast to force the right extension when seq_t is larger. */
            uint64_t sum = (uint64_t)s1 + (uint64_t)s2 + (uint64_t)s3 + (uint64_t)candidate;
            if (isPrime64(sum))
                break;
            ++candidate;
        }
        a_cache[i] = candidate;
        ++a_cache_len;
    }
}

seq_t a(idx_t n)
{
    assert(n >= 1);          /* sequence is defined for n>=1 */
    a_cache_extend(n);
    return a_cache[n-1];
}
