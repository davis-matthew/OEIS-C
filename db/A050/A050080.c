/*
 * A050080  a(n) = floor(a(n-1)/3) if this is not among 0, a(1), ..., a(n-2);
 *          otherwise a(n) = 4*a(n-1).
 * https://oeis.org/A050080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1305, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A050080: a(n) = floor(a(n-1)/3) if that value is not 0 or any previously
 * seen term; otherwise a(n) = 4*a(n-1).  Initial values: a(1)=1, a(2)=4.
 */

#include <stdlib.h>
#include <stdbool.h>

/* cache[1 .. len] holds computed terms.  cache[0] unused. */
static seq_t *cache = NULL;
static idx_t len   = 0;  /* largest pre‑computed index */
static idx_t cap   = 0;  /* allocated capacity */

static void grow_cache(idx_t new_cap)
{
    seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * (new_cap + 1));
    if (!tmp) abort();          /* out of memory */
    for (idx_t i = 1; i <= len; ++i) tmp[i] = cache[i];
    if (cache) free(cache);
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* ensure we have space for up to n */
    if (cap < n) {
        idx_t c = (cap == 0) ? 64 : cap;
        while (c < n) c <<= 1;
        grow_cache(c);
    }

    /* compute all missing terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)4;
        } else {
            seq_t prev = cache[i - 1];
            seq_t cand = prev / (seq_t)3;   /* floor division */

            bool repeated = false;
            if (cand == 0) {
                repeated = true;
            } else {
                for (idx_t j = 1; j <= i - 2; ++j) {
                    if (cache[j] == cand) { repeated = true; break; }
                }
            }
            cache[i] = repeated ? prev * (seq_t)4 : cand;
        }
    }
    len = n;
    return cache[n];
}
