/*
 * A084414  Duplicate of A069691.
 * https://oeis.org/A084414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = smallest prime formed by placing one digit on each side
 *            of n's decimal representation; a(n) = 0 if there is none (search
 *            over 36 candidates)
 * Range:     i8 none, i16 n <= 40, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-07)
 * Helpers:   is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits_idx.h"
#include "../utils/is_prime_u64.h"

static seq_t A084414_pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static inline seq_t A084414_mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static bool A084414_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A084414_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (A084414_fits_u64(n)) return is_prime_u64((uint64_t)n);
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

seq_t a(idx_t n)
{
    int len = (n == 0) ? 1 : num_digits_idx(n);

    seq_t pow10_len1 = A084414_pow10((idx_t)len + 1);
    seq_t n_times_10 = A084414_mul_small((seq_t)n, 10);

    static const unsigned right[4] = {1, 3, 7, 9};

    seq_t best = 0;
    bool found = false;

    for (unsigned left = 1; left <= 9; ++left)
    {
        seq_t left_part = A084414_mul_small(pow10_len1, left);
        for (int i = 0; i < 4; ++i)
        {
            seq_t cand = left_part + n_times_10 + (seq_t)right[i];
            if (A084414_is_prime_big(cand))
            {
                if (!found || cand < best)
                {
                    best = cand;
                    found = true;
                }
            }
        }
    }
    return found ? best : 0;
}
