/*
 * A077684  Squarefree numbers beginning with 8.
 * https://oeis.org/A077684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth squarefree integer whose first decimal digit is 8
 * Range:     u8 n <= 6, u16 n <= 672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches sequentially for numbers that both start with 8
   and are squarefree, caching results for repeated calls. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* State for the incremental generation of the sequence */
static seq_t base_seq   = 1;  /* 10^k */
static seq_t cur_seq    = 8;  /* current candidate */
static seq_t limit_seq  = 8;  /* upper bound of current 10^k block */

/* Generate terms until we have at least target terms cached */
static void generate_until(size_t target) {
    ensure_capacity(target, &cache_cap, (void **)&cache, sizeof(seq_t));
    while (cache_len < target) {
        if (cur_seq > limit_seq) {           /* move to next 10^k block */
            base_seq = base_seq * 10;        /* base *= 10 */
            limit_seq = 9 * base_seq - 1;    /* 9*10^k - 1 */
            cur_seq = 8 * base_seq;          /* 8*10^k */
        }
        if (is_squarefree(cur_seq)) {
            cache[cache_len++] = cur_seq;
        }
        cur_seq += 1;                        /* next candidate */
    }
}

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;   /* offset starts at 1; behaviour for n < 1 is undefined */
    }
    size_t target = (size_t)n;   /* convert to a native size_t for indexing */
    if (cache_len < target) {
        generate_until(target);
    }
    return cache[target - 1];
}
