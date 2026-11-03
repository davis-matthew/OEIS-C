/*
 * A052064  Cubes containing no palindromic substring except single digits.
 * https://oeis.org/A052064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 279, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 54 terms through n = 917 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: cubes containing no palindromic substring except single digits
   (OEIS A052064).  The function returns the n‑th cube that satisfies this
   property, with a(1)=0.  The search is incremental and cached so that
   later calls are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* check whether a decimal representation of <cube> contains any
   palindromic substring of length ≥ 2.  The function works for any
   seq_t that supports %10 and /10. */
static bool cube_has_no_pal_subseq(seq_t cube)
{
    /* extract digits (least‑significant first) */
    uint8_t digits[40];
    int len = 0;
    seq_t tmp = cube;
    do {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp);

    /* reverse to most‑significant first */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t d = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = d;
    }

    /* early rejection: length-2 palindrome (adjacent equal digits) */
    for (int i = 0; i + 1 < len; ++i) {
        if (digits[i] == digits[i + 1])
            return false;
    }
    /* early rejection: length-3 palindrome */
    for (int i = 0; i + 2 < len; ++i) {
        if (digits[i] == digits[i + 2])
            return false;
    }

    /* check longer palindromes (length ≥ 4) */
    for (int i = 0; i < len; ++i) {
        for (int j = i + 3; j < len; ++j) {
            bool pal = true;
            for (int l = i, r = j; l < r; ++l, --r) {
                if (digits[l] != digits[r]) {
                    pal = false;
                    break;
                }
            }
            if (pal)
                return false;
        }
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static cache shared between calls */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_base  = 0;

    while (cache_len < n) {
        seq_t base  = (seq_t)next_base;
        seq_t cube = base * base * base;

        if (cube_has_no_pal_subseq(cube)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = mem_resize(seq_t, cache, new_cap);
                cache       = newp;
                cache_cap   = new_cap;
            }
            cache[cache_len++] = cube;
        }
        ++next_base;
    }
    return cache[n - 1];
}
