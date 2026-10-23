/*
 * A045181  Numbers whose base-5 representation contains exactly two 0's and
 *          one 2.
 * https://oeis.org/A045181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of base‑5 numbers whose representation contains exactly
   two zeros and one 2.  The sequence begins with 50, 127, 135, 175, ….
   The function a(idx_t n) returns the n‑th term (1‑based). */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Cached values; the cache grows automatically. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Next base‑5 length to generate (minimal length is 3). */
static unsigned next_len = 3;

/* Ensure the cache can hold at least 'need' additional values. */
static void ensure_cache_cap(size_t need)
{
    if (cache_len + need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < cache_len + need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);  /* memory exhaustion */
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache. */
static void push_cache(seq_t v)
{
    ensure_cache_cap(1);
    cache[cache_len++] = v;
}

/* Recursive generation of all base‑5 numbers of length L with exactly
   two zeros and one 2.  'pos' is the current digit position (0‑based),
   'zeros' and 'twos' count digits already placed, and 'val' is the value
   built so far. */
static void gen_rec_len(unsigned L,
                        unsigned pos,
                        unsigned zeros,
                        unsigned twos,
                        seq_t val)
{
    if (pos == L) {              /* all positions filled */
        if (zeros == 2 && twos == 1)
            push_cache(val);
        return;
    }

    /* The most significant digit cannot be 0. */
    int start_d = (pos == 0) ? 1 : 0;
    for (int d = start_d; d <= 4; ++d) {
        unsigned nz = zeros + (d == 0);
        unsigned nt = twos  + (d == 2);
        if (nz > 2 || nt > 1)
            continue;                /* violate the required counts */

        /* Prune if remaining positions cannot supply the needed digits. */
        unsigned rem = L - pos - 1;
        unsigned need_z = 2 - nz;
        unsigned need_t = 1 - nt;
        if (need_z + need_t > rem)
            continue;

        seq_t new_val = val * 5 + d;
        gen_rec_len(L, pos + 1, nz, nt, new_val);
    }
}

/* Generate all numbers of a specific length L. */
static void gen_len(unsigned L)
{
    if (L < 3)
        return;
    gen_rec_len(L, 0, 0, 0, (seq_t)0);
}

/* Public function: returns the n‑th term (1‑based).  The cache is
   filled lazily until the n‑th term is available. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined, return 0 as a sentinel */

    while (cache_len < (size_t)n) {
        gen_len(next_len++);        /* generate numbers of the next length */
    }
    return cache[n - 1];
}
