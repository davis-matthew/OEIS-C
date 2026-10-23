/*
 * A037416  Positive numbers having the same set of digits in base 3 and base
 *          4.
 * https://oeis.org/A037416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 5164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generate numbers whose base‑3 and base‑4 representations
   use exactly the same set of digits (digits 0,1,2).  The sequence a(n)
   is the n‑th such positive integer. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *cache          = NULL;  /* stored terms */
static size_t cache_len     = 0;     /* number of terms stored */
static size_t cache_cap     = 0;     /* capacity of cache array */

/* the next integer to test; starts at 1 and increases monotonically */
static seq_t candidate      = 1;

/* return true iff x qualifies: its base‑4 digits (no 3's) set equals
   its base‑3 digits set, both subsets of {0,1,2}. */
static bool qualifies(seq_t x)
{
    unsigned mask4 = 0, mask3 = 0;
    seq_t t = x;

    while (t > 0) {                     /* base‑4 */
        unsigned d = (unsigned)(t % 4);
        if (d == 3) return false;       /* forbidden digit */
        mask4 |= 1u << d;
        t /= 4;
    }

    t = x;
    while (t > 0) {                     /* base‑3 */
        unsigned d = (unsigned)(t % 3);
        mask3 |= 1u << d;
        t /= 3;
    }

    return mask4 == mask3;
}

/* ensure cache has capacity for at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < needed) newcap *= 2;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used in OEIS offset */

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (qualifies(candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len] = candidate;
            cache_len++;
        }
        candidate++;                    /* next integer to test */
    }

    return cache[n - 1];
}
