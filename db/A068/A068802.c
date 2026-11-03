/*
 * A068802  Smaller of two consecutive squares which have no common digits.
 * https://oeis.org/A068802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: full, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068802: Smaller of two consecutive squares which have no common digits. */
/* The function searches successive integer pairs (i,i+1), computes i² and (i+1)²,
   and records those squares whose digit sets are disjoint.  Results are cached
   so that calls with any index order are efficient. */

#include <stdlib.h>
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Compute a bit mask of the decimal digits present in x.  Bit d (0<=d<=9)
   corresponds to digit d.  This works for any seq_t that supports % and /. */
static inline uint16_t digit_mask(seq_t x)
{
    uint16_t mask = 0;
    if (x == 0) {
        return 1U << 0;
    }
    while (x != 0) {
        unsigned d = (unsigned)(x % 10);
        mask |= 1U << d;
        x /= 10;
    }
    return mask;
}

/* Static state: the cache of found squares, the number found and the
   capacity, and the next i to test. */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t  *cache     = NULL;
static idx_t  next_i     = 0;   /* next integer i to test */

seq_t a(idx_t n)
{
    /* Fill the cache until it contains n+1 terms. */
    while (cache_len <= n) {
        seq_t sq   = ((seq_t)next_i) * ((seq_t)next_i);
        seq_t sqn1 = ((seq_t)(next_i + 1)) * ((seq_t)(next_i + 1));
        if ((digit_mask(sq) & digit_mask(sqn1)) == 0) {
            append_candidate(&cache, &cache_len, &cache_cap, sq);
        }
        next_i++;
    }
    return cache[n];
}
