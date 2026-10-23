/*
 * A042181  Denominators of continued fraction convergents to sqrt(615).
 * https://oeis.org/A042181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042181: Denominators of continued fraction convergents to sqrt(615)
   Recurrence: a(n) = 248*a(n-4) - a(n-8) for n > 7
   Initial values:
     a(0)=1, a(1)=1, a(2)=4, a(3)=5,
     a(4)=244, a(5)=249, a(6)=991, a(7)=1240
   This implementation memoises the sequence up to n=10000.
   For larger n a small 8‑slot ring buffer is used. */

#include <stdint.h>
#include <stdbool.h>

/* Static cache for up to 10000 terms. */
static seq_t cache[10001];
static bool cache_init = false;
static idx_t cache_upto = 7;   /* highest index computed in cache */

/* Return a(n). */
seq_t a(idx_t n)
{
    /* Initialise the cache on first use. */
    if (!cache_init) {
        cache[0]  = 1;
        cache[1]  = 1;
        cache[2]  = 4;
        cache[3]  = 5;
        cache[4]  = 244;
        cache[5]  = 249;
        cache[6]  = 991;
        cache[7]  = 1240;
        cache_init = true;
    }

    /* If the requested index is within the cached range, return it. */
    if (n <= 10000) {
        if (n > cache_upto) {
            /* Extend the cache up to n. */
            for (idx_t i = cache_upto + 1; i <= n; ++i) {
                cache[i] = cache[i-4] * (seq_t)248 - cache[i-8];
            }
            cache_upto = n;
        }
        return cache[n];
    }

    /* For n > 10000: use a ring buffer of 8 values. */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = cache[i];
    for (idx_t i = 8; i <= n; ++i) {
        seq_t new_val = buf[i % 8] * (seq_t)248 - buf[(i-8) % 8];
        buf[i % 8] = new_val;
    }
    return buf[n % 8];
}
