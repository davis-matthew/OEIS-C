/*
 * A088268  Palindromes in A007530.
 * https://oeis.org/A088268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search -- enumerate decimal palindromes k in increasing order
 *            and keep those with k, k+2, k+6, k+8 all prime (terms of
 *            A007530). For k > 5 every A007530 term is 11 mod 30, so its last
 *            digit, and therefore its first digit, is 1. Even-length
 *            palindromes > 11 are divisible by 11, so only 11 is teste...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A088268_terms[256];
static idx_t A088268_cnt = 0;
static int A088268_len = 0;     /* last digit length fully processed */
static bool A088268_done = false;

static uint64_t A088268_p10(int e) {
    uint64_t r = 1;
    while (e-- > 0) r *= 10;
    return r;
}

static void A088268_test(uint64_t k) {
    if (is_prime_u64(k) && is_prime_u64(k + 2) && is_prime_u64(k + 6) && is_prime_u64(k + 8)) {
        if (A088268_cnt < 256) A088268_terms[A088268_cnt++] = k;
    }
}

/* process all palindromes of the next digit length */
static void A088268_next_length(void) {
    int len = A088268_len + 1;
    A088268_len = len;
    if (len > 17) { A088268_done = true; return; }
    if (len == 1) {
        uint64_t d;
        for (d = 1; d <= 9; d++) A088268_test(d);
        return;
    }
    if (len % 2 == 0) {
        if (len == 2) A088268_test(11);
        return;
    }
    {
        int m = (len + 1) / 2;
        uint64_t lo = A088268_p10(m - 1), hi = 2 * lo, h;
        for (h = lo; h < hi; h++) {
            uint64_t pal = h, t = h / 10;
            while (t > 0) { pal = pal * 10 + t % 10; t /= 10; }
            A088268_test(pal);
        }
    }
}

seq_t a(idx_t n) {
    while (A088268_cnt < n && !A088268_done) A088268_next_length();
    if (n < 1 || n > A088268_cnt) return (seq_t)0;
    return (seq_t)A088268_terms[n - 1];
}
