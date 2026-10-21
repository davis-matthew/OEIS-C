/*
 * A057534  Start with 61; to get the next term, divide by the smallest prime
 *          factor < 17 if it exists, else multiply by 17 and add 1.
 * https://oeis.org/A057534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=61; for n>0, let p be the smallest prime <17 dividing
 *            a(n‑1) (if any). Then a(n)=a(n‑1)/p; otherwise a(n)=17*a(n‑1)+1.
 *            The sequence has period 84
 * Range:     u8 n <= 0, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_val.h"

static seq_t cache[84];
static bool init_done = false;

static void build_cache(void)
{
    seq_t cur = (seq_t)61;
    cache[0] = cur;
    for (int i = 1; i < 84; ++i) {
        cur = next_val(cur);
        cache[i] = cur;
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        build_cache();
    unsigned int idx = (unsigned int)(n % 84ULL);
    return cache[idx];
}
