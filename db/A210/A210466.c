/*
 * A210466  Length of the shortest path to the hardest to obtain volumes in
 *          the two jugs problem.
 * https://oeis.org/A210466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the shortest solution to the “two jugs
 *            problem” for the n‑th coprime pair (x,y) with x>y>1, enumerated
 *            in increasing x and then y
 * Range:     u8 n <= 5023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_max_steps, gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the maximum over all goal volumes
   1..x of the minimal number of steps needed to reach that
   volume with the jugs (x,y).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"
#include "../utils/compute_max_steps.h"

/* ----------  cache and enumeration logic  ---------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_x = 3;
static idx_t next_y = 2;
static bool initialized = false;

static void cache_ensure(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    if (new_cap < new_len) new_cap = new_len;
    seq_t *tmp = (seq_t*)seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void advance_pair(void)
{
    ++next_y;
    if (next_y >= next_x) { ++next_x; next_y = 2; }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    if (!initialized)
    {
        initialized = true;
        next_x = 3;
        next_y = 2;
        cache_len = 0;
        cache_cap = 0;
        cache = NULL;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* skip non–coprime pairs */
        while (gcd((seq_t)next_x, (seq_t)next_y) != (seq_t)1)
            advance_pair();

        seq_t val = compute_max_steps((seq_t)next_x, (seq_t)next_y);

        cache_ensure(cache_len + 1);
        cache[cache_len++] = val;

        advance_pair();   /* move to the next pair for the next loop */
    }

    return cache[cache_len - 1];
}
