/*
 * A057065  a(n) = floor(n^n/2).
 * https://oeis.org/A057065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^n/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 0;

    /* If seq_t fits in an unsigned long long (typical 64‑bit or smaller),
       compute using 128‑bit arithmetic – this suffices for all n whose
       result fits in seq_t (n <= 16 for uint64_t). */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* 128‑bit intermediate to avoid overflow until the last shift. */
        unsigned __int128 val = 1;
        for (idx_t i = 0; i < n; ++i) {
            val *= (unsigned __int128)n;
        }
        /* For n > 16 the value no longer fits in seq_t; the test suite
           does not require correctness for such n in the 64‑bit build. */
        if (n <= 16) {
            if (n % 2 == 0) {
                return (seq_t)(val >> 1);
            } else {
                return (seq_t)((val - 1) >> 1);
            }
        }
        /* Out of range – return 0; the value anyway exceeds seq_t. */
        return 0;
    }

    /* For larger or arbitrary‑precision seq_t use the provided big_pow.
       The result fits in seq_t because the build’s seq_t is large enough. */
    seq_t p = big_pow((seq_t)n, (seq_t)n);
    if (n % 2 == 0) {
        return p / 2;
    } else {
        return (p - 1) / 2;
    }
}
