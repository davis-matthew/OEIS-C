/*
 * A057259  Numbers n such that n | 10^n + 9^n + 8^n + 7^n + 6^n + 5^n.
 * https://oeis.org/A057259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer n such that n | 10^n + 9^n + 8^n + 7^n
 *            + 6^n + 5^n
 * Range:     u8 n <= 17, u16 n <= 105, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* modular exponentiation: base^exp mod mod, works for 64‑bit values. */
static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Cache for found terms */
static seq_t *terms   = NULL;
static size_t terms_len = 0;   /* number of terms stored */
static size_t terms_cap = 0;   /* allocated capacity */

/* Next integer to test for the property */
static idx_t next_candidate = 1;

/* Append a new term to the cache, expanding if necessary */
static void push_term(seq_t value)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)value;
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but harmless */

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        uint64_t m = (uint64_t)next_candidate;
        uint64_t sum = 0;

        /* Compute 5^m + 6^m + ... + 10^m modulo m */
        for (uint64_t base = 5; base <= 10; ++base) {
            sum += mod_pow64(base, m, m);
        }

        if ((sum % m) == 0) {
            push_term((seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
