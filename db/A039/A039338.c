/*
 * A039338  Numbers whose base-8 representation has the same nonzero number of
 *          1's and 4's.
 * https://oeis.org/A039338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-8 representation has the same nonzero number
 *            of 1's and 4's. Computed by brute‑force enumeration
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;    /* cached sequence terms          */
static size_t cache_cap = 0;   /* allocated array capacity       */
static size_t cache_len = 0;   /* number of terms currently stored */
static idx_t last_checked = 0; /* last integer that was tested   */

/* Return true iff x has equal non‑zero counts of 1’s and 4’s in base‑8. */
static bool accept(seq_t x)
{
    int c1 = 0, c4 = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        int d = (int)(t % (seq_t)8);
        if (d == 1) c1++;
        else if (d == 4) c4++;
        t /= (seq_t)8;
    }
    return (c1 > 0) && (c1 == c4);
}

/* Grow the cache array in a power‑of‑two step. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term. */
    if (n == (idx_t)0) return 0;          /* undefined below offset */

    if ((size_t)n <= cache_len)           /* already computed? */
        return cache[(size_t)(n - 1)];

    /* Generate terms until we have at least n of them. */
    while ((size_t)n > cache_len) {
        last_checked++;                   /* next integer to test */
        seq_t val = (seq_t)last_checked;
        if (accept(val)) {
            if (cache_len >= cache_cap) grow_cache();
            cache[cache_len++] = val;
        }
    }
    return cache[(size_t)(n - 1)];
}
