/*
 * A235807  Numbers k such that k^3 has one or more occurrences of exactly
 *          five different digits.
 * https://oeis.org/A235807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 396, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 73 terms through n = 979 (2026-11-03)
 * Helpers:   digit_counts, ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search to compute sequence A235807:
   Numbers k such that k^3 contains exactly five distinct digits
   in its decimal representation.                                 */

#include <stddef.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Memoized list of terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static idx_t next_k = 1;             /* next candidate to test */

    if (n == 0) return 0;  /* behaviour below offset is irrelevant */

    while (terms_len < n) {
        seq_t cube = ipow_idx((seq_t)next_k, (unsigned)3);

        unsigned char cnt[10];
        memset(cnt, 0, sizeof(cnt));
        digit_counts(cube, cnt);

        int distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d]) ++distinct;

        if (distinct == 5)
            append_candidate(&terms, &terms_len, &terms_cap, (seq_t)next_k);

        ++next_k;
    }

    return terms[n - 1];
}
