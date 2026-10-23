/*
 * A132722  Numbers with k decimal digits that are also k-th powers.
 * https://oeis.org/A132722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = the n-th number that has k decimal digits and is a k-th
 *            power
 * Range:     u8 n <= 17, u16 n <= 27, u32 n <= 38, u64 n <= 48, u128 n <= 49,
 *            big any n
 * Helpers:   digit_len, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates all such numbers by iterating over k (exponent) and
   base, storing them in increasing order of k then base. The sequence has
   only 49 terms, so the cache is static. */
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/digit_len.h"

static seq_t _cache[60];
static idx_t _count = 0;

seq_t a(idx_t n)
{
    if (_count == 0) {
        /* Precompute the full sequence once. */
        for (idx_t k = 1; k <= 60; ++k) {
            bool any = false;
            for (uint64_t base = 1; base <= 9; ++base) {
                seq_t val = pow_uint(base, k);
                if (digit_len(val) == k) {
                    _cache[_count++] = val;
                    any = true;
                }
            }
            /* Stop when no term is found for this exponent (k > 1). */
            if (!any && k > 1)
                break;
        }
    }

    if (n == 0 || n > _count)
        return 0;
    return _cache[n - 1];
}
