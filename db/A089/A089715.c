/*
 * A089715  a(n) = {A089713(n)+A070219(n)}/2.
 * https://oeis.org/A089715
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (A089713(n) + A070219(n))/2; computed by direct search
 *            over concatenations n||m
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 48 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static seq_t A089715_concat(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pw = 1;
    seq_t tmp = part_b;
    while (tmp > 0) {
        pw *= 10;
        tmp /= 10;
    }
    return part_a * pw + part_b;
}

static bool A089715_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A089715_is_prime(seq_t n) {
    if (n < 2) return false;
    if (A089715_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;

        seq_t x = 1;
        seq_t pw = b % n;
        seq_t ex = d;
        while (ex > 0) {
            if (ex & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            ex >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t A089713_a(idx_t n)
{
    if (n <= 2) return 0;
    idx_t m = n;
    while (m > 1) {
        --m;
        seq_t val = A089715_concat(n, m);
        if (A089715_is_prime(val)) return val;
    }
    return 0;
}

static seq_t A070219_a(idx_t n)
{
    idx_t m = n + 1;
    for (;; ++m) {
        seq_t cand = A089715_concat(n, m);
        if (A089715_is_prime(cand))
            return cand;
    }
}

seq_t a(idx_t n)
{
    seq_t x = A089713_a(n);
    seq_t y = A070219_a(n);
    return x / 2 + y / 2 + ((x & 1) & (y & 1));
}
