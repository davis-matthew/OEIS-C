/*
 * A090980  From the set of numbers > 1, go on deleting n smallest numbers
 *          relatively prime to n that were not deleted earlier; a(n) = the
 *          largest number deleted.
 * https://oeis.org/A090980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A090980 by simulating the deletion process:
   for each n = 1,2,3,… delete the n smallest integers >1 that
   are relatively prime to n and have not been deleted earlier.
   a(n) is the largest number deleted in the nth step. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Euclidean algorithm on idx_t (uint64_t).  This avoids overflow when
   seq_t is narrower. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Bit-set helpers --------------------------------------------------- */
static uint8_t *deleted_bits = NULL;          /* dynamic bit array */
static idx_t deleted_bits_len = 0;            /* number of bits allocated */
static size_t deleted_bytes_len = 0;          /* number of bytes allocated */

/* Ensure the bit array can hold index 'idx' (inclusive) */
static void ensure_bit_capacity(idx_t idx)
{
    /* idx is ≥0.  Bits are 0‑based.  Ensure bits 0…idx exist. */
    if (!deleted_bits) {                     /* first allocation */
        deleted_bits_len = 8;
        deleted_bytes_len = 1;
        deleted_bits = (uint8_t*)calloc(1, 1);
        if (!deleted_bits) abort();
        return;
    }
    while ((idx + 1) > deleted_bits_len) {
        deleted_bits_len <<= 1;              /* double size */
        size_t new_bytes = (deleted_bits_len + 7) >> 3;
        uint8_t *tmp = (uint8_t*)realloc(deleted_bits, new_bytes);
        if (!tmp) abort();
        memset(tmp + deleted_bytes_len, 0, new_bytes - deleted_bytes_len);
        deleted_bits = tmp;
        deleted_bytes_len = new_bytes;
    }
}

static inline bool is_deleted(idx_t x)
{
    return (deleted_bits[x >> 3] >> (x & 7)) & 1;
}
static inline void set_deleted(idx_t x)
{
    deleted_bits[x >> 3] |= 1 << (x & 7);
}

/* Cached a(n) values ------------------------------------------------ */
static seq_t *a_cache = NULL;               /* dynamic cache */
static size_t a_cache_size = 0;             /* allocated size */
static idx_t a_max_computed = 0;

/* Main function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, just return 0 */

    /* Ensure cache has room for up to n */
    if (!a_cache) {
        a_cache_size = 2;
        a_cache = (seq_t*)calloc(a_cache_size, sizeof(seq_t));
        if (!a_cache) abort();
    }
    if ((size_t)n >= a_cache_size) {
        size_t sz = a_cache_size;
        while (sz <= (size_t)n) sz <<= 1;
        seq_t *tmp = (seq_t*)realloc(a_cache, sz * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_size = sz;
    }

    /* Return already computed value */
    if (n <= a_max_computed) return a_cache[(size_t)n];

    /* Compute values up to n */
    for (idx_t step = a_max_computed + 1; step <= n; ++step) {
        idx_t count = 0;
        seq_t largest = 0;

        for (idx_t num = 2; ; ++num) {
            ensure_bit_capacity(num);
            if (!is_deleted(num)) {
                if (gcd_idx(num, step) == 1) {
                    set_deleted(num);
                    largest = num;
                    if (++count == step) break;
                }
            }
        }
        a_cache[(size_t)step] = largest;
    }

    a_max_computed = n;
    return a_cache[(size_t)n];
}
