/*
 * A056750  Numbers k such that k | 7^k + 6^k + 5^k + 4^k + 3^k + 2^k + 1^k.
 * https://oeis.org/A056750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search for integers k such that
   k divides 7^k + 6^k + 5^k + 4^k + 3^k + 2^k + 1^k. */
/* The function a(idx_t n) returns the n‑th element (1‑based) of OEIS A056750. */

#include <stdlib.h>
#include <stdbool.h>

/* Compute base^exp mod mod using only seq_t operations. */
static seq_t modpow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Test whether k satisfies the required divisibility condition. */
static bool good_k(idx_t k)
{
    seq_t mod = (seq_t)k;
    seq_t sum = 0;
    for (int b = 1; b <= 7; ++b) {
        seq_t t = modpow((seq_t)b, k, mod);
        sum = (sum + t) % mod;
    }
    return sum == 0;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static idx_t nextk = 1;

    /* Find terms until we have at least n of them. */
    while (cached < n) {
        if (good_k(nextk)) {
            size_t new_cap = cached + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache[cached] = (seq_t)nextk;
            ++cached;
        }
        ++nextk;
    }
    return cache[n - 1];
}
