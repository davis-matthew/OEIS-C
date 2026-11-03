/*
 * A104381  Numbers k such that 10^(k-1) == 1 (mod k).
 * https://oeis.org/A104381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 6616, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 7310 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 10^(k-1) ≡ 1 (mod k) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a given k satisfies 10^(k-1) ≡ 1 (mod k). */
static inline bool is_good(uint64_t k)
{
    if (k == 1) return true;          /* 1 is always included */
    uint64_t mod = k;
    uint64_t res = 1 % mod;
    uint64_t base = 10 % mod;
    uint64_t e = k - 1;
    while (e) {
        if (e & 1) {
            __int128 prod = (__int128)res * base;
            res = (uint64_t)(prod % mod);
        }
        e >>= 1;
        if (e) {
            __int128 prod = (__int128)base * base;
            base = (uint64_t)(prod % mod);
        }
    }
    return res == 1;
}

/* Compute the nth term of A104381. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap = 0;
    static uint64_t next_k = 1;   /* next k to test */

    /* Ensure we have at least n terms stored. */
    if (terms_size < (size_t)n) {
        /* Grow storage if needed. */
        if (terms_cap == 0) {
            terms_cap = 1024;
            terms = mem_new(seq_t, terms_cap);
        }
        while (terms_cap < (size_t)n) {
            terms_cap *= 2;
            terms = mem_resize(seq_t, terms, terms_cap);
        }
        /* Generate terms until we have n of them. */
        while (terms_size < (size_t)n) {
            if (is_good(next_k))
                terms[terms_size++] = (seq_t)next_k;
            ++next_k;
        }
    }
    return terms[n - 1];
}
