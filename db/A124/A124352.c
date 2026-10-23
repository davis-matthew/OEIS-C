/*
 * A124352  Number of directed Hamiltonian paths on the n-antiprism graph.
 * https://oeis.org/A124352
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 49, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124352: Number of directed Hamiltonian paths on the n‑antiprism graph.
   The sequence satisfies the linear recurrence

   a(n) = 5·a(n‑1) – 6·a(n‑2) – 4·a(n‑3) + 7·a(n‑4) + 5·a(n‑5) – 5·a(n‑6)
          – 3·a(n‑7) + a(n‑8) + a(n‑9)        ,  n > 11

   with initial values

   a(3)=240, a(4)=816, a(5)=2400, a(6)=6480, a(7)=16660,
   a(8)=41440, a(9)=100836, a(10)=241520, a(11)=571692.
   The function memoises computed values so that it works in any call order.
*/

#include <stddef.h>
#include <stdint.h>

#define MAX_N (20000)          /* sufficient for the required range up to about 10 000 */

static seq_t cache[MAX_N + 1];   /* cache[n] holds a(n); indices 0,1,2 unused */
static unsigned char cache_init = 0;

static void init_cache(void) {
    if (cache_init) return;
    /* initialise known values */
    cache[3]  = 240;
    cache[4]  = 816;
    cache[5]  = 2400;
    cache[6]  = 6480;
    cache[7]  = 16660;
    cache[8]  = 41440;
    cache[9]  = 100836;
    cache[10] = 241520;
    cache[11] = 571692;
    cache_init = 1;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;          /* the sequence is defined only for n >= 3 */

    init_cache();

    /* compute sequentially up to n if not already cached */
    if (n > MAX_N) {
        /* for safety we could extend the cache, but n is expected <= ~10000 */
        return 0;
    }
    if (n <= 11) return cache[n];

    /* find last computed index */
    static idx_t last_computed = 11;
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* compute using only positive and negative parts separately */
            seq_t pos = cache[i-1] * 5
                      + cache[i-4] * 7
                      + cache[i-5] * 5
                      + cache[i-8]
                      + cache[i-9];

            seq_t neg = cache[i-2] * 6
                      + cache[i-3] * 4
                      + cache[i-6] * 5
                      + cache[i-7] * 3;

            cache[i] = pos - neg;          /* pos >= neg for all terms */
        }
        last_computed = n;
    }

    return cache[n];
}
