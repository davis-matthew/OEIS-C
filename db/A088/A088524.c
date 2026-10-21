/*
 * A088524  Greatest twin prime that does not exceed n (starting at n=3).
 * https://oeis.org/A088524
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the greatest twin prime ≤ n (starting at a(3)=3)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* Terms below the offset are unspecified; return 0 for safety */
    if (n < 3) return 0;

    for (idx_t p = n; ; --p) {
        if (isPrime(p)) {
            bool twin = false;
            /* check p-2 if it is non‑negative */
            if (p >= 5) twin = twin || isPrime(p - 2);
            /* check p+2 only if addition cannot overflow */
            if (p <= UINT64_MAX - 2) twin = twin || isPrime(p + 2);
            if (twin) return (seq_t)p;
        }
        /* stop once we reach the smallest possible twin prime */
        if (p == 3) break;
    }
    /* Should never be reached for n ≥ 3 with proper input */
    return 0;
}
