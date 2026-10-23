/*
 * A198378  Smallest prime with multiplicative digital root value 0 <= n <= 9.
 * https://oeis.org/A198378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest prime p with multiplicative digital root MDR(p)
 *            = n (for 0 ≤ n ≤ 9, a(n) = 0 otherwise)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_product, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_product.h"

static seq_t cache[10] = {0};   /* first prime for each MDR value */
static bool ready = false;      /* cache initialized flag */

/* Compute the cache on first use */
static void init_cache(void)
{
    uint64_t p = 2;          /* candidate prime */
    int found = 0;           /* how many MDR values have been settled */

    while (found < 10) {
        if (is_prime(p)) {
            seq_t x = (seq_t)p;
            /* Reduce to single-digit multiplicative digital root */
            while (x >= 10) {
                x = digit_product(x);
            }
            int d = (int)x;   /* 0 ≤ d ≤ 9 */

            if (cache[d] == 0) {      /* first prime with this MDR */
                cache[d] = (seq_t)p;
                ++found;
            }
        }
        ++p;
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) init_cache();

    if (n >= 10) return 0;          /* sequence defined only for 0..9 */
    return cache[(int)n];
}
