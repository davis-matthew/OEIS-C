/*
 * A187054  Numbers that are not the sum of three powerful numbers (A001694).
 * https://oeis.org/A187054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187054 – numbers that are NOT the sum of three powerful numbers
   (a(n) is defined by searching all sums of three powerful numbers). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are supplied by the build; we do not redeclare them. */

/* maximal value we inspect – enough to capture all known terms (last term 119). */
#define MAX_SUM 200

/* cache of the sequence – the sequence is known to have at most 12 terms. */
static seq_t  seq_cache[20];
static size_t seq_len  = 0;
static bool   seq_done = false;

/* Test whether m is a powerful number (every prime factor exponent ≥ 2). */
static bool is_powerful(uint32_t m)
{
    if (m <= 1) return true;          /* 1 is powerful by convention */
    uint32_t x = m;
    for (uint32_t p = 2; (uint64_t)p * p <= x; ++p) {
        if (x % p == 0) {
            uint32_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            if (cnt < 2) return false;
        }
    }
    /* If a prime factor remains, its exponent is 1 -> not powerful. */
    return x == 1;
}

/* Build the sequence cache only once. */
static void build_sequence(void)
{
    if (seq_done) return;

    /* Collect all powerful numbers up to MAX_SUM. */
    uint32_t pow_list[64];
    size_t pow_cnt = 0;
    for (uint32_t n = 1; n <= MAX_SUM; ++n) {
        if (is_powerful(n)) pow_list[pow_cnt++] = n;
    }

    /* Mark all numbers that can be expressed as sum of three powerful numbers. */
    bool represent[MAX_SUM + 1] = { false };
    for (size_t i = 0; i < pow_cnt; ++i) {
        for (size_t j = i; j < pow_cnt; ++j) {
            for (size_t k = j; k < pow_cnt; ++k) {
                uint32_t sum = pow_list[i] + pow_list[j] + pow_list[k];
                if (sum > MAX_SUM) break;
                represent[sum] = true;
            }
        }
    }

    /* Collect the non‑representable numbers into the cache. */
    for (uint32_t v = 1; v <= MAX_SUM; ++v) {
        if (!represent[v]) {
            seq_cache[seq_len++] = (seq_t)v;
            if (seq_len >= sizeof(seq_cache) / sizeof(seq_cache[0])) break;
        }
    }

    seq_done = true;
}

/* Public interface required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */

    /* Build cache on first call. */
    if (!seq_done) build_sequence();

    /* Return the requested term if it exists. */
    if ((size_t)(n - 1) < seq_len) {
        return seq_cache[(size_t)(n - 1)];
    }

    /* For n beyond the known length the sequence is undefined – return 0. */
    return (seq_t)0;
}
