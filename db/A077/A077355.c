/*
 * A077355  Squares whose internal digits form a square.
 * https://oeis.org/A077355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   ipow_i32, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077355 – Squares whose internal digits form a square.
    a(n) = the n-th perfect square whose internal decimal digits form a perfect square.
    (OEIS offset 1)
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ----- power‑of‑ten helper --------------------------------------- */
static seq_t pow10_of_len_minus_one(int e)   /* 10^e,  e >= 0 */
{
    seq_t p = (seq_t)1;
    for (int i = 0; i < e; ++i)
        p *= (seq_t)10;
    return p;
}

/* ----- internal digits square test -------------------------------- */
static bool internal_is_square(seq_t n)
{
    /* Number of decimal digits of n */
    int len = num_digits(n);
    if (len <= 2)            /* empty internal part ⇒ 0² */
        return true;

    seq_t pow10len1 = pow10_of_len_minus_one(len - 1);  /* 10^(len‑1) */

    /* Remove first and last digit: (n % 10^(len-1)) / 10 */
    seq_t inner = (n % pow10len1) / (seq_t)10;

    seq_t r = floor_sqrt(inner);
    return r * r == inner;
}

/* ----- cache for computed terms ---------------------------------- */
static seq_t *cache = NULL;      /* array of found squares        */
static size_t cache_len = 0;     /* number of valid entries       */
static size_t cache_cap = 0;     /* allocated capacity            */
static idx_t next_k = 0;         /* next k to test (k²)          */

seq_t a(idx_t n)
{
    if (n == 0)           /* safety: OEIS offset is 1, so return 0 */
        return (seq_t)0;

    /* Grow cache until it holds at least n terms */
    while (cache_len < n) {
        seq_t k   = (seq_t)next_k;    /* current k (as seq_t) */
        seq_t sq  = k * k;            /* k²     */

        if (internal_is_square(sq)) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {
                    /* allocation failure – abort */
                    abort();
                }
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
        ++next_k;
    }
    return cache[n - 1];   /* a(1) is cache[0]  */
}
