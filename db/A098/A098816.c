/*
 * A098816  a(1)=0, a(2)=1, a(n) = ceiling((3/2) * |a(n-1) - a(n-2)|).
 * https://oeis.org/A098816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(n) = ceil((3/2)*|a(n-1)-a(n-2)|)
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the OEIS sequence A098816.
   Uses the period 193 starting from n=19 for O(1) computation after
   an initial O(211)-time precomputation. */
seq_t a(idx_t n)
{
    /* Static cache for the first 211 terms (1..211 inclusive) */
    static bool initialized = false;
    static seq_t cache[212];  /* cache[0] unused */

    if (!initialized) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        for (idx_t i = 3; i <= 211; ++i) {
            seq_t x = cache[i - 1];
            seq_t y = cache[i - 2];
            seq_t diff = (x > y) ? (x - y) : (y - x);
            unsigned long long d = (unsigned long long)diff;
            unsigned long long val;
            if ((d & 1ULL) == 0ULL)          /* even difference  */
                val = (3ULL * d) / 2ULL;
            else                              /* odd difference   */
                val = (3ULL * d + 1ULL) / 2ULL;
            cache[i] = (seq_t)val;
        }
        initialized = true;
    }

    /* For n >= 19, the sequence is periodic with period 193. */
    if (n <= 211) {
        return cache[n];
    } else {
        idx_t k = 19 + ((n - 19) % (idx_t)193ULL);
        return cache[k];
    }
}
