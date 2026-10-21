/*
 * A070008  Arithmetic mean of distinct primes dividing n is a power of 2
 *          (powers of 2 are not left out).
 * https://oeis.org/A070008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 593, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A070008: arithmetic mean of distinct primes dividing n is a power of 2.
   a(n) is the nth positive integer k>1 such that the average of the distinct prime
   divisors of k equals 2^m for some m≥0.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"

static uint32_t *spf = NULL;          /* smallest prime factor table */
static size_t    spf_cap = 0;         /* size of spf table   */

static seq_t *seq = NULL;             /* cached sequence values   */
static size_t seq_size = 0;           /* number of values stored */
static size_t seq_cap  = 0;           /* capacity of seq buffer  */
static uint64_t last_checked = 1;     /* last integer checked     */

/* Build spf up to 'limit', inclusive. Assumes spf==NULL or realloc needed. */
static void build_spf(size_t limit)
{
    uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf = new_spf;
    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; (uint64_t)i * (uint64_t)i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_cap = limit;
}

/* Ensure spf table covers at least 'need'.  Expands if necessary. */
static void ensure_spf(size_t need)
{
    if (need <= spf_cap) return;
    size_t new_cap = spf_cap ? spf_cap * 2 : 1 << 20;   /* start at 1M */
    while (new_cap < need) new_cap <<= 1;
    build_spf(new_cap);
}

/* Append new value into seq buffer, expanding if required. */
static void append_seq(seq_t val)
{
    if (seq_size == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq) exit(EXIT_FAILURE);
        seq = new_seq;
        seq_cap = new_cap;
    }
    seq[seq_size++] = val;
}

/* Return the nth term of the sequence, with 1-based indexing. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; guard against misuse */

    /* Initialize spf and seq on first call. */
    if (spf == NULL) build_spf(1 << 20);   /* 1 048 576 – sufficient for n≤10 000 */

    /* Generate terms until we have at least 'n' of them. */
    while (seq_size < (size_t)n)
    {
        ++last_checked;                    /* check next integer */
        ensure_spf((size_t)last_checked);   /* guarantee spf coverage */

        uint64_t temp  = last_checked;
        seq_t sum = 0;
        size_t k = 0;
        uint32_t last_p = 0;

        /* Enumerate distinct prime divisors using the spf table. */
        while (temp > 1)
        {
            uint32_t p = spf[temp];
            if (p != last_p) {
                sum += p;
                ++k;
                last_p = p;
            }
            while (temp % p == 0) temp /= p;
        }

        if (k > 0) {
            if ((sum % (seq_t)k) == 0) {
                seq_t avg = sum / (seq_t)k;
                if (is_power_of_two(avg)) {
                    append_seq(last_checked);
                }
            }
        }
    }

    return seq[n - 1];
}
