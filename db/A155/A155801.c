/*
 * A155801  Nontrivial "Strobogrammatic" primes, the same "upside-down" in at
 *          least one base b with 2 <= b <= 10.
 * https://oeis.org/A155801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth prime that is strobogrammatic in at least one base 2
 *            ≤ b ≤ 10
 * Range:     u8 n <= 14, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_cache, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_cache.h"

/* Mapping of decimal digits to their 180° rotation image.
   -1 denotes an invalid digit. */
static const int ROTATE_MAP[10] = { 0, 1, -1, -1, -1, -1, 9, -1, 8, 6 };

/* Test whether n is strobogrammatic in a single base. */
static bool is_strobogrammatic_in_base(seq_t n, int base)
{
    int digits[256];
    int len = 0;

    if (n == 0) return false;

    seq_t tmp = n;
    const seq_t sb = (seq_t)base;
    while (tmp > 0) {
        int d = (int)(tmp % sb);
        digits[len++] = d;
        tmp /= sb;
    }

    for (int i = 0; i < len; ++i) {
        int j   = len - 1 - i;
        int d1  = digits[i];
        int d2  = digits[j];
        int rot = ROTATE_MAP[d2];
        if (rot == -1 || d1 != rot) return false;
    }
    return true;
}

/* Test whether n is strobogrammatic in any base 2…10. */
static bool is_strobogrammatic_any_base(seq_t n)
{
    for (int base = 2; base <= 10; ++base) {
        if (is_strobogrammatic_in_base(n, base)) return true;
    }
    return false;
}

/* Return the nth strobogrammatic prime (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t cache_cnt   = 0;
    static seq_t cur         = 2;        /* next prime candidate */

    /* Ensure sufficient storage. */
    ensure_cache(n, &cache, &cache_cap);

    /* Generate primes until we have n terms. */
    while (cache_cnt < n) {
        if (is_prime_big(cur) && is_strobogrammatic_any_base(cur)) {
            cache[cache_cnt++] = cur;
        }

        seq_t prev = cur;
        if (cur == 2) cur = 3;
        else          cur += 2;            /* skip even numbers            */

        /* Detect wrap‑around on unsigned seq_t. */
        if (cur <= prev) break;
    }

    if (cache_cnt >= n) return cache[(size_t)(n - 1)];
    /* If the sequence cannot be continued within the capacity of seq_t,
       return 0 as a safe failure value. */
    return (seq_t)0;
}
