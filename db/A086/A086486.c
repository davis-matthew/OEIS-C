/*
 * A086486  Numbers k such that the sum of the distinct prime divisors divides
 *          rad(k)=A007947(k).
 * https://oeis.org/A086486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086486: a(n) = the n-th integer k such that the sum of distinct prime
 * divisors of k divides rad(k). This implementation computes the terms
 * by brute‑force search, using a sieve of smallest prime factors to
 * factor each candidate efficiently. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* cache of already computed terms */
static seq_t *seq_arr  = NULL;
static idx_t  seq_len  = 0;
static idx_t  seq_cap  = 0;

/* smallest prime factor sieve */
static uint32_t *spf     = NULL;   /* indices up to spf_cap */
static idx_t    spf_cap  = 0;      /* maximum index that spf covers */

/* next integer to test for membership */
static idx_t next_check = 2;

/* push a new term into the cache, resizing as needed */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 8 : seq_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        seq_arr = new_arr;
        seq_cap = (idx_t)new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* extend the spf array so that it covers numbers up to 'limit' */
static void extend_spf(idx_t limit)
{
    if (limit <= spf_cap) return;

    size_t new_cap = (size_t)(limit + 1);
    uint32_t *new_spf = (uint32_t *)calloc(new_cap, sizeof(uint32_t));
    if (!new_spf) exit(EXIT_FAILURE);

    /* initialise to the number itself */
    for (size_t i = 2; i < new_cap; ++i) new_spf[i] = (uint32_t)i;

    /* sieve for smallest prime factor */
    for (size_t i = 2; i * i < new_cap; ++i) {
        if (new_spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j < new_cap; j += i) {
                if (new_spf[j] == (uint32_t)j)
                    new_spf[j] = (uint32_t)i;
            }
        }
    }

    free(spf);
    spf     = new_spf;
    spf_cap = limit;
}

/* test whether 'k' satisfies the defining condition */
static bool satisfies(idx_t k)
{
    seq_t rad = 1;
    seq_t sum = 0;
    uint32_t prev = 0;

    idx_t x = k;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        if (p != prev) {
            sum += (seq_t)p;
            rad *= (seq_t)p;
            prev = p;
        }
        while (x % p == 0) x /= p;
    }
    return (rad % sum) == 0;
}

/* generate terms until we have at least 'target' terms in the cache */
static void generate_upto(idx_t target)
{
    /* start at the next integer to be tested */
    while (seq_len < target) {
        if (next_check > spf_cap) extend_spf(next_check * 2);

        if (satisfies(next_check))
            seq_push((seq_t)next_check);

        ++next_check;
    }
}

/* public function as required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    generate_upto(n);
    return seq_arr[n - 1];
}
