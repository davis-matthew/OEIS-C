/*
 * A131297  a(n) = ds_11(a(n-1))+ds_11(a(n-2)), a(0)=0, a(1)=1; where
 *          ds_11=digital sum base 11.
 * https://oeis.org/A131297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0, a(1)=1, a(n)=ds_11(a(n-1))+ds_11(a(n-2)) for n>=2 *
 *            where ds_11(x) is the sum of the digits of x in base 11. *
 *            Implementation exploits the fact that for n>=3 the sequence is *
 *            periodic with period 60, so only the first 63 terms are *
 *            pre‑computed and arbitrary n is reduced modulo 60
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* static cache for the first 63 terms: 0 … 62 */
static seq_t cache[63];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* one‑time initialization */
    if (!initialized) {
        cache[0] = 0;
        cache[1] = 1;
        for (idx_t i = 2; i < 63; ++i) {
            /* a(i) = a(i-1) + a(i-2) - 10 * (a(i-1)/11 + a(i-2)/11) */
            seq_t x = cache[i-1];
            seq_t y = cache[i-2];
            cache[i] = x + y - 10 * (x / 11 + y / 11);
        }
        initialized = true;
    }

    /* direct return for small indices */
    if (n < 63) {
        return cache[n];
    }

    /* for n >= 3 the sequence repeats every 60 terms */
    idx_t rem = (n - 3) % 60;           /* 0 … 59 */
    return cache[rem + 3];              /* map back to 3 … 62 */
}
