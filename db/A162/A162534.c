/*
 * A162534  A positive integer k is included if every length of the runs of
 *          0's and 1's in the binary representation of k divides k.
 * https://oeis.org/A162534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A162534 – numbers whose binary run lengths all divide the number.
   The function generates terms by brute‑force search, caching already found
   values for fast repeated access. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true iff every run of equal bits in the binary representation of k
   has a length that divides k. */
static bool has_property(seq_t k)
{
    if (k == 0) return false;          /* sequence starts at 1 */

    /* Compute the number of bits in k. */
    size_t bitlen = 0;
    seq_t tmp = k;
    while (tmp) {
        bitlen++;
        tmp >>= 1;
    }

    /* A single‑bit number trivially satisfies the condition. */
    if (bitlen == 1) return true;

    /* Scan bits from most significant to least significant. */
    seq_t cur_bit = (k >> (bitlen - 1)) & 1;
    seq_t run_len = 1;
    size_t pos = bitlen - 2;

    while (true) {
        seq_t bit = (k >> pos) & 1;
        if (bit == cur_bit) {
            run_len++;
        } else {
            if (k % run_len != 0) return false;
            cur_bit = bit;
            run_len = 1;
        }
        if (pos == 0) {
            if (k % run_len != 0) return false;
            break;
        }
        pos--;
    }
    return true;
}

/* Return the n‑th term of the sequence.  The offset is 1: a(1) = 1. */
seq_t a(idx_t n)
{
    /* Cache of found terms to allow arbitrary access order. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_k = 1;

    if (n == 0) return 0;  /* undefined for this offset, but harmless */

    /* Generate terms until we have at least n cached. */
    while (cache_len < (size_t)n) {
        if (has_property(next_k)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap < 64 ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
