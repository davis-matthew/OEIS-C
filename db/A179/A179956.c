/*
 * A179956  a(n) is the smallest integer that contains a(n-1) but does not
 *          begin with a(n-1).
 * https://oeis.org/A179956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 10^(n-1) + a(n-1) for n>2, a(1)=1, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n == 0) {
        return 0;   /* undefined offset, return 0 */
    }

    if (cache_len < n) {
        grow_seq(&cache, &cache_cap, (size_t)n);

        if (cache_len < 1) {
            cache[0] = 1;
            cache_len = 1;
        }
        if (cache_len < 2) {
            cache[1] = 21;
            cache_len = 2;
        }

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t p = pow10((idx_t)(i - 1));
            cache[i - 1] = p + cache[i - 2];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
