/*
 * A153055  Numbers k such that the binary expansion of k is a substring of
 *          the binary expansion of 1/k.
 * https://oeis.org/A153055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   my_bitlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A153055 – numbers k such that the binary expansion of k is
 *  a substring of the binary expansion of 1/k.
 *  Search: test each integer k for membership by a long‑division
 *  with binary digits and a simple pattern search.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/my_bitlen.h"

/* check whether the binary representation of k occurs in the binary
 * expansion of 1/k (terminating or repeating part). */
static bool satisfies(idx_t cand)
{
    if (cand < 2) return false;

    seq_t k = (seq_t)cand;
    unsigned L = my_bitlen(k);          /* length of binary representation of k */

    /* pattern bits of k: most significant first */
    uint8_t *pat = (uint8_t*)malloc(L);
    if (!pat) return false;
    seq_t tmp = k;
    for (int i = (int)L - 1; i >= 0; --i) {
        pat[i] = ((tmp & (seq_t)1) ? 1 : 0);
        tmp >>= 1;
    }

    /* arrays for remainders and bits of 1/k */
    int *pos = (int*)malloc((size_t)k * sizeof(int));
    if (!pos) { free(pat); return false; }
    memset(pos, 0xFF, (size_t)k * sizeof(int));
    pos[1] = 0;                 /* initial remainder before any bit */

    uint8_t *bits = NULL;       /* bits of expansion of 1/k */
    size_t bits_cap = 0, bits_len = 0;
    seq_t r = (seq_t)1;
    size_t pre_len = 0, per_len = 0, per_start = 0;
    bool terminated = false;

    while (true) {
        /* produce next bit */
        r <<= 1;
        uint8_t bit = (r / k) ? 1 : 0;
        r %= k;

        /* store bit */
        if (bits_len == bits_cap) {
            size_t new_cap = bits_cap ? bits_cap << 1 : 64;
            uint8_t *tmpv = (uint8_t*)realloc(bits, new_cap);
            if (!tmpv) { free(pat); free(pos); free(bits); return false; }
            bits = tmpv; bits_cap = new_cap;
        }
        bits[bits_len++] = bit;

        /* check for end conditions */
        if (r == (seq_t)0) {          /* terminating */
            terminated = true;
            pre_len = bits_len;
            break;
        }
        if (pos[(size_t)r] != -1) {   /* cycle detected */
            per_start = pos[(size_t)r];
            per_len = bits_len - per_start;
            break;
        }
        pos[(size_t)r] = (int)bits_len;   /* record position after this bit */
    }

    /* ------------- search ------------------------------------ */

    /* helper to search for pattern inside bits[0..len-1] */
    bool found = false;
    if (bits_len >= L) {
        for (size_t i = 0; i + L <= bits_len; ++i) {
            size_t j;
            for (j = 0; j < L; ++j) {
                if (bits[i + j] != pat[j]) break;
            }
            if (j == L) { found = true; break; }
        }
    }

    if (!found && !terminated && per_len > 0) {
        /* search inside one period plus overlap */
        size_t ext_len = per_len + L - 1;
        uint8_t *ext = (uint8_t*)malloc(ext_len);
        if (ext) {
            memcpy(ext, bits + per_start, per_len);
            for (size_t i = 0; i < L - 1; ++i) {
                ext[per_len + i] = bits[per_start + i];
            }
            for (size_t i = 0; i + L <= ext_len; ++i) {
                size_t j;
                for (j = 0; j < L; ++j) {
                    if (ext[i + j] != pat[j]) break;
                }
                if (j == L) { found = true; break; }
            }
            free(ext);
        }
    }

    free(pat);
    free(pos);
    free(bits);
    return found;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* static cache for previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 2;        /* next candidate to test */

    while (cache_len < (size_t)n) {
        /* find next admissible k */
        while (!satisfies((idx_t)next_k)) ++next_k;

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 64;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */
            cache = tmp; cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[n - 1];
}
