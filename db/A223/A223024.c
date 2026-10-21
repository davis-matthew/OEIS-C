/*
 * A223024  Numbers k such that 3^k is odious (A000069).
 * https://oeis.org/A223024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A223024: numbers k such that 3^k is odious.
 * Compute k for a given n (1‑based).  The computation is done by
 * generating successive powers of 3 and testing the parity of their popcount.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* parity32: parity of ones in a 32‑bit word */
static inline unsigned parity32(uint32_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_parity(x);
#else
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1U;
#endif
}

/* seq_t a(idx_t n) : return the n‑th k with odd popcount(3^k) */
seq_t a(idx_t n)
{
    /* static cache of computed terms (a(1), a(2), …) */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* number of terms stored */
    static size_t terms_cap = 0;   /* array capacity */

    /* state for generating 3^k */
    static idx_t current_k = 0;     /* last processed exponent */
    static uint32_t *power   = NULL;/* little‑endian limbs of 3^k */
    static size_t power_sz   = 0;   /* number of limbs in use */
    static size_t power_cap  = 0;   /* allocated limbs */

    /* one‑time initialization */
    if (!terms_len) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
        terms_len = 1;
        terms[0] = (seq_t)0;              /* a(1) = 0 */

        power_cap = 8;
        power = (uint32_t *)malloc(power_cap * sizeof(uint32_t));
        if (!power) abort();
        power_sz = 1;
        power[0] = 1;                      /* 3^0 = 1 */
    }

    /* if we already know the term, return it */
    if (n <= terms_len)
        return terms[n - 1];

    /* ensure the terms array can hold at least n elements */
    if (n > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }

    /* generate terms until we reach the requested index */
    while (terms_len < n) {
        /* next exponent */
        current_k++;

        /* multiply current power by 3 */
        uint64_t carry = 0;
        for (size_t i = 0; i < power_sz; ++i) {
            uint64_t t = (uint64_t)power[i] * 3 + carry;
            power[i] = (uint32_t)t;
            carry = t >> 32;
        }

        if (carry) {
            if (power_sz == power_cap) {
                size_t new_cap = power_cap ? power_cap * 2 : 8;
                uint32_t *tmp = (uint32_t *)realloc(power, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                power = tmp;
                power_cap = new_cap;
            }
            power[power_sz++] = (uint32_t)carry;
        }

        /* compute parity of popcount of 3^k */
        unsigned parity = 0;
        for (size_t i = 0; i < power_sz; ++i)
            parity ^= parity32(power[i]);

        if (parity) { /* odious */
            /* grow terms array if necessary before inserting */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)current_k;
        }
    }

    return terms[n - 1];
}
