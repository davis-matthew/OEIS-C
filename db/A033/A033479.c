/*
 * A033479  3x+1 sequence beginning at 9.
 * https://oeis.org/A033479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Collatz sequence starting at 9. Recurrence: a(0)=9; for
 *            k>0, a(k) = 3*a(k-1)+1 if a(k-1) is odd, a(k-1)/2 if a(k-1) is
 *            even. For n>19 the sequence repeats with period 3: a(n) = a(n-3)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the first 20 values on first call
   (using the rule above) and then uses the period for larger n. */

#include <stdbool.h>

static seq_t cache[20];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* initialize the cache on first use */
    if (!initialized) {
        cache[0] = (seq_t)9;
        for (idx_t i = 1; i < 20; ++i) {
            seq_t prev = cache[i - 1];
            if (prev & (seq_t)1)
                cache[i] = prev * (seq_t)3 + (seq_t)1;
            else
                cache[i] = prev / (seq_t)2;
        }
        initialized = true;
    }

    if (n < 20)
        return cache[n];

    /* a(n) = a(n-3) for n > 19.  Use 17,18,19 as the base cycle. */
    idx_t idx = 17 + (n - 17) % (idx_t)3;
    return cache[idx];
}
