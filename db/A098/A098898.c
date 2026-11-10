/*
 * A098898  Duplicate of A098768.
 * https://oeis.org/A098898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th k with sigma(k) < 2k (deficient) and
 *            sigma(reverse(k)) > 2*reverse(k) (abundant); found by a search
 *            with a cache
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"

static uint64_t A098898_sigma(uint64_t n)
{
    uint64_t s = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            s += d;
            uint64_t e = n / d;
            if (e != d) s += e;
        }
    }
    return s;
}

static uint64_t A098898_rev(uint64_t x)
{
    uint64_t r = 0;
    while (x) {
        r = r * 10 + x % 10;
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t size = 0, cap = 0;
    static uint64_t next = 1;

    while (size < (size_t)n) {
        uint64_t c = next++;
        if (A098898_sigma(c) < 2 * c) {
            uint64_t r = A098898_rev(c);
            if (r > 0 && A098898_sigma(r) > 2 * r) {
                if (size + 1 > cap) {
                    size_t nc = cap ? cap * 2 : 64;
                    cache = mem_resize(seq_t, cache, nc);
                    cap = nc;
                }
                cache[size++] = (seq_t)c;
            }
        }
    }
    return cache[(size_t)(n - 1)];
}
