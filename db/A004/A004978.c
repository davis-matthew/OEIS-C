/*
 * A004978  a(n) = least integer m > a(n-1) such that m - a(n-1) != a(j) -
 *          a(k) for all j, k less than n; a(1) = 1, a(2) = 2.
 * https://oeis.org/A004978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; a(n) = a(n-1) + M, where M is the smallest
 *            positive integer that is not a difference a(j) - a(k) with n > j
 *            > k > 0. Computed with a greedy search that keeps a bitset of
 *            the differences seen so far
 * Range:     u8 n <= 16, u16 n <= 220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *terms = NULL;   /* terms[1..count] */
static idx_t terms_cap = 0;
static idx_t count = 0;

static uint64_t *bits = NULL;    /* bitset of differences seen so far */
static uint64_t bits_words = 0;
static uint64_t min_missing = 1; /* smallest positive integer not in the set */

static void ensure_bits(uint64_t maxval) {
    uint64_t need = maxval / 64 + 1;
    if (need <= bits_words) return;
    uint64_t nw = bits_words ? bits_words : 1024;
    while (nw < need) nw *= 2;
    uint64_t *nb = (uint64_t *)realloc(bits, (size_t)nw * sizeof(uint64_t));
    memset(nb + bits_words, 0, (size_t)(nw - bits_words) * sizeof(uint64_t));
    bits = nb;
    bits_words = nw;
}

static void add_term(uint64_t v) {
    if (count + 1 >= terms_cap) {
        idx_t nc = terms_cap ? terms_cap * 2 : 1024;
        terms = (uint64_t *)realloc(terms, (size_t)(nc + 1) * sizeof(uint64_t));
        terms_cap = nc;
    }
    count++;
    terms[count] = v;
    ensure_bits(v);
    for (idx_t k = 1; k < count; k++) {
        uint64_t d = v - terms[k];
        bits[d >> 6] |= (uint64_t)1 << (d & 63);
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (count == 0) {
        add_term(1);
        add_term(2);
    }
    while (count < n) {
        ensure_bits(min_missing + 64);
        while ((bits[min_missing >> 6] >> (min_missing & 63)) & 1) {
            min_missing++;
            ensure_bits(min_missing + 64);
        }
        add_term(terms[count] + min_missing);
    }
    return (seq_t)terms[n];
}
