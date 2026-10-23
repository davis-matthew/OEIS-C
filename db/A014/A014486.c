/*
 * A014486  List of totally balanced sequences of 2n binary digits written in
 *          base 10. Binary expansion of each term contains n 0's and n 1's
 *          and reading from left to right (the most significant to the least
 *          significant bit), the number of 0's never exceeds the number of
 *          1's.
 * https://oeis.org/A014486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   a(n) = the n-th Dyck word (balanced 0/1 sequence with N zeros
 *            and N ones, never more 0's than 1's in any prefix) written in
 *            base 10. The terms are enumerated in increasing binary order,
 *            length 2k increasing with k. No simple closed formula is known
 * Range:     u8 n <= 22, u16 n <= 2055, u32 n <= 9654, u64 n <= 9654,
 *            u128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache: all computed terms in order of index */
static seq_t *cache = NULL;     /* pointer to sequence array */
static size_t cache_size = 0;   /* number of terms already stored */
static size_t cache_cap   = 0;   /* current capacity of cache array */

/* next k (half‑length) for which we have not generated any terms yet */
static idx_t next_k = 1;

/* ensure cache has room for at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need)
        newcap <<= 1;

    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr)               /* allocation failed – leave cache unchanged */
        return;

    cache = newptr;
    cache_cap = newcap;
}

/* recursive generation of all Dyck words of length L (bits)
   pos          – next bit position to set (0‑based)
   ones_used    – number of 1 bits already placed
   zeros_used   – number of 0 bits already placed
   current      – numeric value of the prefix so far   */
static void generate_dyck(int L, int pos, int ones_used, int zeros_used,
                          seq_t current)
{
    if (pos == L) {
        /* full word built – store it */
        ensure_cache_capacity(cache_size + 1);
        if (cache_cap >= cache_size + 1) {   /* allocation succeeded */
            cache[cache_size++] = current;
        }
        return;
    }

    /* try putting a 0 first (lexicographically smaller) */
    if (zeros_used < ones_used && zeros_used < L / 2) {
        generate_dyck(L, pos + 1, ones_used,
                      zeros_used + 1, current << 1);
    }

    /* then try putting a 1 */
    if (ones_used < L / 2) {
        generate_dyck(L, pos + 1, ones_used + 1,
                      zeros_used, (current << 1) | 1);
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* initialise cache with the empty word if necessary */
    if (cache_size == 0) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)        /* out of memory – return 0 */
            return (seq_t)0;
        cache[0] = (seq_t)0;   /* a(0) = 0 */
        cache_size = 1;
    }

    /* return already computed term if available */
    if ((idx_t)n < (idx_t)cache_size)
        return cache[n];

    /* generate further terms until we have at least n+1 values */
    while ((idx_t)n >= (idx_t)cache_size) {
        int k = (int)next_k;          /* current half‑length (integer) */
        int L = 2 * k;                /* total bits for this length */

        /* start with the leading 1 bit */
        generate_dyck(L, 1, 1, 0, (seq_t)1);

        ++next_k;
    }

    return cache[n];
}
