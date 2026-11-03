/*
 * A235809  Numbers k such that k^3 has one or more occurrences of exactly
 *          seven different digits.
 * https://oeis.org/A235809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 428, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 957 (2026-11-03)
 * Helpers:   digit_counts, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th integer k for which k³ contains exactly seven distinct
   decimal digits.  The sequence is defined by search; no closed formula
   is known. */
/* https://oeis.org/A235809 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of found terms and current scanning position */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_k = 1;   /* next k to test */

    /* Grow cache to hold at least n terms */
    while (terms_len < n)
    {
        seq_t cube = ipow((seq_t)next_k, (seq_t)3);

        unsigned char cnt[10];
        for (int i = 0; i < 10; ++i) cnt[i] = 0;
        digit_counts(cube, cnt);

        int distinct = 0;
        for (int i = 0; i < 10; ++i)
            if (cnt[i] > 0)
                ++distinct;

        if (distinct == 7)
        {
            /* ensure capacity */
            if (terms_len >= terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) abort();          /* allocation failed */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
