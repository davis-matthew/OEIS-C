/*
 * A045099  Numbers whose base-4 representation contains exactly two 1's and
 *          four 2's.
 * https://oeis.org/A045099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑4 representation contains exactly two 1's
   and four 2's.  The values are generated in increasing numerical order by
   increasing base‑4 length and lexicographic digit order. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache    = NULL;   /* dynamically grown cache of terms   */
static size_t cache_size = 0;
static size_t cache_cap  = 0;    /* current capacity of cache          */
static idx_t  next_len   = 6;    /* minimal length (2×1 + 4×2)         */

/* Append a generated value to the cache, enlarging the array if necessary. */
static void push_val(seq_t v)
{
    if (cache_size == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
    cache[cache_size++] = v;
}

/* Recursive generator: build all base‑4 numbers of a given length `len`
   that contain exactly `rem1` digits '1' and `rem2` digits '2'.  Remaining
   positions are filled with 0 or 3.  `pos` is the current digit index
   (0 = most significant).  `val` holds the value constructed so far. */
static void gen_rec(idx_t len, idx_t pos, seq_t val, int rem1, int rem2)
{
    if (pos == len) {
        if (rem1 == 0 && rem2 == 0)
            push_val(val);
        return;
    }
    /* Prune branches that cannot satisfy the remaining digit counts. */
    if (len - pos < (idx_t)(rem1 + rem2))
        return;

    for (int d = 0; d < 4; ++d) {
        if (pos == 0 && d == 0)          /* no leading zero */
            continue;
        if (d == 1 && rem1 == 0)         /* no more '1' allowed */
            continue;
        if (d == 2 && rem2 == 0)         /* no more '2' allowed */
            continue;

        int nrem1 = rem1 - (d == 1);
        int nrem2 = rem2 - (d == 2);
        seq_t nval = val;
        nval = (nval * (seq_t)4) + (seq_t)d;
        gen_rec(len, pos + 1, nval, nrem1, nrem2);
    }
}

/* Generate all numbers of a specific base‑4 length. */
static void generate_len(idx_t len)
{
    gen_rec(len, 0, (seq_t)0, 2, 4);   /* exactly two '1's and four '2's */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0, harmless */

    /* Extend the cache until it holds at least n terms. */
    while (cache_size < (size_t)n) {
        generate_len(next_len);
        ++next_len;
    }

    return cache[n - 1];
}
