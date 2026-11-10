/*
 * A050098  Numbers k such that A050096(k) < A050096(k+1).
 * https://oeis.org/A050098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A050096(k) < A050096(k+1); A050096(1) =
 *            1, A050096(n) = floor(A050096(n-1)/3) if positive and not
 *            already in the sequence, else 8*A050096(n-1). Found by scanning
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A050096_cache = NULL;
static size_t A050096_len = 0;
static size_t A050096_cap = 0;

static void A050096_ensure(size_t needed)
{
    if (needed <= A050096_cap)
        return;
    size_t new_cap = A050096_cap ? A050096_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *arr = mem_new(seq_t, new_cap);
    if (!arr)
        exit(1);
    for (size_t i = 0; i < A050096_len; ++i)
        arr[i] = A050096_cache[i];
    if (A050096_cache)
        mem_free(A050096_cache);
    A050096_cache = arr;
    A050096_cap = new_cap;
}

static int A050096_seen(seq_t v, size_t upto)
{
    for (size_t i = 0; i < upto; ++i)
        if (A050096_cache[i] == v)
            return 1;
    return 0;
}

static seq_t A050096_a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (A050096_len == 0) {
        A050096_ensure(64);
        A050096_cache[0] = (seq_t)1;
        A050096_len = 1;
    }
    while (A050096_len < (size_t)n) {
        seq_t prev = A050096_cache[A050096_len - 1];
        seq_t cand = prev / (seq_t)3;
        seq_t next;
        if (cand > (seq_t)0 && !A050096_seen(cand, A050096_len))
            next = cand;
        else
            next = prev * (seq_t)8;
        if (A050096_len == A050096_cap)
            A050096_ensure(A050096_cap << 1);
        A050096_cache[A050096_len] = next;
        ++A050096_len;
    }
    return A050096_cache[n - 1];
}

static idx_t *A050098_terms = NULL;
static size_t A050098_cnt = 0;
static size_t A050098_cap = 0;
static idx_t A050098_next_k = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    while (A050098_cnt < (size_t)n) {
        idx_t k = A050098_next_k++;
        if (A050096_a(k) < A050096_a(k + 1)) {
            if (A050098_cnt == A050098_cap) {
                size_t nc = A050098_cap ? A050098_cap * 2 : 64;
                idx_t *na = (idx_t *)malloc(nc * sizeof(idx_t));
                if (!na)
                    exit(1);
                for (size_t i = 0; i < A050098_cnt; ++i)
                    na[i] = A050098_terms[i];
                if (A050098_terms)
                    free(A050098_terms);
                A050098_terms = na;
                A050098_cap = nc;
            }
            A050098_terms[A050098_cnt++] = k;
        }
    }
    return (seq_t)A050098_terms[n - 1];
}
