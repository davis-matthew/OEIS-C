/*
 * A129141  a(1)=a(2)=1. a(n) = the n-th integer from among those positive
 *          integers coprime to (a(n-1)*a(n-2)).
 * https://oeis.org/A129141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer coprime to a(n-1)*a(n-2) with
 *            a(1)=a(2)=1. The implementation uses memoization and avoids
 *            overflow by testing coprimality against the two previous terms
 *            separately (gcd(a,b)=1 iff gca=gcd(a,c)=1 and gcd(b,c)=1)
 * Range:     u8 n <= 63, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_CACHE 20001   /* enough for n up to ~10 000, extendable */

static seq_t seq_cache[MAX_CACHE];
static idx_t cache_used = 0;
static bool cache_initialized = false;

static void init_cache(void)
{
    if (!cache_initialized) {
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)1;
        cache_used  = 2;
        cache_initialized = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    init_cache();

    /* Return cached value if already computed */
    if (cache_used >= n) return seq_cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        seq_t count = 0;
        seq_t cand  = (seq_t)1;

        while (1) {
            /* Check coprimality with the two previous terms */
            if (gcd(seq_cache[i-1], cand) == (seq_t)1 &&
                gcd(seq_cache[i-2], cand) == (seq_t)1) {

                ++count;
                if (count == i) {          /* the i‑th coprime */
                    seq_cache[i] = cand;
                    break;
                }
            }
            ++cand;
        }
    }
    cache_used = n;
    return seq_cache[n];
}
