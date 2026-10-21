/*
 * A068359  Binomial(2k,k) when the first digit of binomial(2k,k) is 1.
 * https://oeis.org/A068359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th central binomial coefficient C(2k,k) whose first
 *            decimal digit is 1. The offset is 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   digit_len, ensure_capacity, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for out‑of‑range calls */

    /* static state for memoisation */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;

    /* current k (starting with 0) and the exact value of C(2k,k) */
    static idx_t k            = 0;
    static unsigned __int128 current128 = 1;   /* 1 for k = 0 */

    /* largest value that can be represented in seq_t */
    static seq_t max_val       = 0;
    static bool   max_val_init = false;
    if (!max_val_init) {
        for (size_t i = 0; i < sizeof(seq_t) * 8; ++i)
            max_val = (max_val << 1) + 1;
        max_val_init = true;
    }

    /* generate until we have the requested term */
    while (cache_len < (size_t)n) {
        /* if current value no longer fits in seq_t, we cannot store it */
        if (current128 > (unsigned __int128)max_val)
            break;

        seq_t current = (seq_t)current128;

        /* test if the first decimal digit is 1 */
        idx_t len  = digit_len(current);
        seq_t pow  = pow10((idx_t)(len - 1));
        seq_t fd   = current / pow;
        if (fd == (seq_t)1) {
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = current;
        }

        /* advance to the next central binomial coefficient */
        idx_t k_next = k + 1;
        unsigned __int128 factor = (unsigned __int128)(4 * k_next - 2);
        current128 = current128 * factor / (unsigned __int128)k_next;
        k = k_next;
    }

    if (cache_len < (size_t)n) return 0;  /* term too large for seq_t */
    return cache[n - 1];
}
