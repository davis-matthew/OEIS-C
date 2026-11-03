/*
 * A036745  Squares including each digit exactly once.
 * https://oeis.org/A036745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: full, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036745:
   a(n) is the n‑th square whose decimal representation contains
   each digit 0–9 exactly once (a 0‑9 pandigital number). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check whether x (assumed to fit in 64 bits) is a 0‑9 pandigital integer. */
static bool is_pandigital_u64(uint64_t x)
{
    uint16_t mask = 0;
    int digits   = 0;
    while (x > 0) {
        int d = x % 10;
        if (mask & (1u << d))
            return false;          /* repeated digit */
        mask |= 1u << d;
        x /= 10;
        ++digits;
    }
    return (digits == 10 && mask == 0x3FFu);   /* all digits 0–9 present exactly once */
}

static bool _a_built = false;
static seq_t * _a_cache = NULL;
static size_t  _a_count = 0;

/* Build the cache of all pandigital squares up to the 10‑digit upper bound. */
static void build_cache(void)
{
    /* 10‑digit pandigital numbers lie between 1 023 456 789 and */
    /* 9 876 543 210, so any square larger than that cannot be in the
       sequence. */
    const uint64_t LIMIT = 9876543210ULL;
    size_t capacity = 100;                     /* at most 87 terms */
    _a_cache = mem_new(seq_t, capacity);
    _a_count = 0;

    for (uint64_t m = 1; m * m <= LIMIT; ++m) {
        uint64_t sq = m * m;
        if (is_pandigital_u64(sq)) {
            if (_a_count == capacity) {
                capacity *= 2;
                _a_cache = mem_resize(seq_t, _a_cache, capacity);
            }
            _a_cache[_a_count++] = (seq_t)sq;
        }
    }
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    if (!_a_built) {
        build_cache();
        _a_built = true;
    }
    if (n == 0 || n > _a_count)
        return 0;          /* out of range   */
    return _a_cache[n - 1];
}
