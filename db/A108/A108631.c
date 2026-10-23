/*
 * A108631  Semiprimes with nonprime digits (no digits 2,3,5,7 in semiprimes).
 * https://oeis.org/A108631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth semiprime whose decimal digits are not 2,3,5,7
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   allowed_digits, is_semiprime_uint64, isprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/allowed_digits.h"
#include "../utils/is_semiprime_uint64.h"
#include "../utils/isprime_uint64.h"

/* sequence a(n) with offset 1 */
seq_t a(idx_t n)
{
    /* cache of previously found terms */
    static uint64_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static uint64_t last_checked = 4;     /* next integer to test */

    if (n == 0)      /* safety for 0‑based calls, though offset is 1 */
        return 0;

    while (cache_size < n) {
        if (allowed_digits(last_checked) && is_semiprime_uint64(last_checked)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? (cache_cap << 1) : 16;
                cache = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
            }
            cache[cache_size++] = last_checked;
        }
        ++last_checked;
    }

    return (seq_t)cache[n - 1];
}
