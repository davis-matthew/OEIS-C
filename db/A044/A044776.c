/*
 * A044776  Numbers n such that string 6,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers n such that string 6,3 occurs in the base 10
 *            representation of n but not of n+1
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool contains63(seq_t x)
{
    if (x == 0)
        return false;
    seq_t prev = 0;
    bool first = true;
    while (x > 0) {
        seq_t d = x % 10;
        if (!first) {
            if (prev == 3 && d == 6)
                return true;
        }
        first = false;
        prev = d;
        x = x / 10;
    }
    return false;
}

#define CACHE_CAP 200000

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static seq_t last_num = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, any value acceptable */

    while (cache_len < n) {
        last_num = last_num + 1;
        if (contains63(last_num)) {
            seq_t next = last_num + 1;
            if (!contains63(next)) {
                if (cache_len < CACHE_CAP)
                    cache[cache_len++] = last_num;
                /* ignore overflow beyond capacity – not expected in tests */
            }
        }
    }
    return cache[n - 1];
}
