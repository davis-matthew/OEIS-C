/*
 * A083478  a(n) is the smallest k > 0 such that k*Palindrome(n)+1 is a
 *          palindrome.
 * https://oeis.org/A083478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k > 0 such that k*Palindrome(n)+1 is a
 *            palindrome where Palindrome(n) is the n‑th base‑10 palindrome.
 *            The function searches k from 1 upwards
 * Range:     u8 none, u16 n <= 42, u32 n <= 130, u64 n <= 130, u128 n <= 130,
 *            big any n
 * Helpers:   is_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_pal.h"

/* Helper: precompute powers of ten up to 10^20 */
static seq_t pow10_cache[21];
static bool pow10_inited = false;
static void init_pow10(void) {
    if (!pow10_inited) {
        pow10_cache[0] = 1;
        for (int i = 1; i < 21; ++i)
            pow10_cache[i] = pow10_cache[i - 1] * 10;
        pow10_inited = true;
    }
}
static seq_t get_pow10(int exp) {
    init_pow10();
    return pow10_cache[exp];
}

/* Reverse the decimal digits of a number */
static seq_t reverse_seq(seq_t x) {
    seq_t rev = 0;
    while (x != 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

/* Compute the n-th base‑10 palindrome without cache (used for n > 10000) */
static seq_t compute_pal(idx_t n) {
    init_pow10();
    seq_t cumulative = 0;
    for (int len = 1; ; ++len) {
        int h = (len + 1) / 2;
        seq_t count_len = 9 * get_pow10(h - 1);
        if (cumulative + count_len >= n) {
            seq_t offset = n - cumulative;          /* 1‑based offset */
            seq_t first_half_start = get_pow10(h - 1);
            seq_t first_half = first_half_start + (seq_t)(offset - 1);
            seq_t pal;
            if (len % 2 == 0) {                    /* even length */
                int h_even = len / 2;
                seq_t rev = reverse_seq(first_half);
                pal = first_half * get_pow10(h_even) + rev;
            } else {                                /* odd length */
                int h_odd = (len + 1) / 2;
                seq_t mh = first_half / 10;        /* exclude middle digit */
                seq_t rev = reverse_seq(mh);
                pal = first_half * get_pow10(h_odd - 1) + rev;
            }
            return pal;
        }
        cumulative += count_len;
    }
}

/* Cache the first 10,000 palindromes for repeated queries */
static seq_t pal_cache[10001];
static bool pal_cache_inited = false;
static seq_t get_pal(idx_t n) {
    if (!pal_cache_inited) {
        for (idx_t i = 1; i <= 10000; ++i)
            pal_cache[i] = compute_pal(i);
        pal_cache_inited = true;
    }
    if (n <= 10000)
        return pal_cache[n];
    return compute_pal(n);
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t P = get_pal(n);
    for (idx_t k = 1; ; ++k) {
        seq_t cand = (seq_t)k * P + 1;
        if (is_pal(cand))
            return (seq_t)k;
    }
}
