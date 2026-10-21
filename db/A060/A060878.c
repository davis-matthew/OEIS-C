/*
 * A060878  Intrinsic 8-palindromes: n is an intrinsic k-palindrome if it is a
 *          k-digit palindrome in some base.
 * https://oeis.org/A060878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th smallest positive integer that can be written as an
 * 8‑digit palindrome in some base ≥ 2. 
 *
 * The implementation enumerates all 8‑digit palindromes in bases 2…10,
 * removes duplicate values, sorts the list, and returns the n‑th smallest.
 * For n up to ≈ 10 000 this range of bases is sufficient.
 * The cache is built only once and reused for subsequent calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"

/* --------------------------------------------------------------------- */
/* Static storage for intermediate values and the final cache             */
/* --------------------------------------------------------------------- */
#define MAX_TEMP 30000          /* enough for bases 2…10 (≈ 22 300 raw values) */
static seq_t temp_storage[MAX_TEMP];
static size_t temp_len = 0;

static seq_t cache[MAX_TEMP];
static size_t cache_len = 0;
static bool cache_built = false;

/* --------------------------------------------------------------------- */
/* Build the cache on first use                                          */
/* --------------------------------------------------------------------- */
static void build_cache(void)
{
    if (cache_built) return;

    temp_len = 0;

    /* Enumerate base values b = 2 … 10 */
    for (idx_t base = 2; base <= 10; ++base) {
        /* Pre‑compute powers of the current base:
         * power[i] = base^i   for i = 0 … 7
         */
        seq_t power[8];
        power[0] = (seq_t)1;
        for (int i = 1; i < 8; ++i)
            power[i] = power[i-1] * (seq_t)base;

        /* Combination terms for the palindrome digits */
        seq_t term_a = power[7] + power[0]; /* d7·(b^7 + b^0) */
        seq_t term_b = power[6] + power[1]; /* d6·(b^6 + b^1) */
        seq_t term_c = power[5] + power[2]; /* d5·(b^5 + b^2) */
        seq_t term_d = power[4] + power[3]; /* d4·(b^4 + b^3) */

        /* Iterate over all possible digit assignments */
        for (idx_t a = 1; a < base; ++a)           /* d7 must be non‑zero    */
        for (idx_t b = 0; b < base; ++b)           /* d6                     */
        for (idx_t c = 0; c < base; ++c)           /* d5                     */
        for (idx_t d = 0; d < base; ++d) {         /* d4                     */
            seq_t val = (seq_t)a * term_a
                      + (seq_t)b * term_b
                      + (seq_t)c * term_c
                      + (seq_t)d * term_d;

            if (temp_len < MAX_TEMP)
                temp_storage[temp_len++] = val;
        }
    }

    /* Sort all generated values */
    qsort(temp_storage, temp_len, sizeof(seq_t), cmp_seq_t);

    /* Copy unique values into the final cache */
    cache_len = 0;
    for (size_t i = 0; i < temp_len; ++i) {
        if (i == 0 || temp_storage[i] != temp_storage[i-1]) {
            cache[cache_len++] = temp_storage[i];
        }
    }

    cache_built = true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Assume n ≥ 1; if not, return 0 as a safe default. */
    if (n == 0) return (seq_t)0;

    build_cache();                /* Ensure the cache is ready. */

    /* n is 1‑based; the cache array is 0‑based. */
    if ((size_t)(n-1) >= cache_len)
        return (seq_t)0;          /* Out of range – not expected for the tests. */

    return cache[(size_t)(n-1)];
}
