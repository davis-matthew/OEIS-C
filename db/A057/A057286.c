/*
 * A057286  Numbers k such that k | 11^k + 10^k + 9^k + 8^k + 7^k + 6^k + 5^k
 *          + 4^k + 3^k.
 * https://oeis.org/A057286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A057286 by brute–force search.
   A(n) is the nth positive integer k such that
   k divides 11^k + 10^k + 9^k + 8^k + 7^k + 6^k + 5^k + 4^k + 3^k. */

#include <stdlib.h>
#include "../utils/mem.h"

static inline seq_t modpow_small_base(idx_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = (seq_t)base % mod;
    while (exp > 0) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_k = 1;

    /* Extend the terms array if necessary */
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 8 : terms_cap << 1;
        terms = mem_resize(seq_t, terms, terms_cap);
    }

    while (terms_len < n) {
        /* Only odd k can satisfy the condition (sum of odd powers mod 2 = 1). */
        if ((next_k & 1) == 0) {
            ++next_k;
            continue;
        }

        seq_t k_seq = (seq_t)next_k;
        seq_t sum = 0;
        for (int base = 3; base <= 11; ++base) {
            seq_t p = modpow_small_base(base, next_k, k_seq);
            sum += p;
            sum %= k_seq;
        }

        if (sum == 0) {          /* k divides the sum */
            terms[terms_len++] = k_seq;
        }
        ++next_k;
    }

    return terms[n - 1];
}
