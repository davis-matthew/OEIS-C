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
 * Helpers:   append_candidate, digit_counts, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search to compute sequence A235807:
   Numbers k such that k^3 contains exactly five distinct digits
   in its decimal representation.                                 */

#include <stddef.h>
#include <string.h>

#include "../utils/append_candidate.h"
#include "../utils/digit_counts.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Memoized list of terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static idx_t next_k = 1;             /* next candidate to test */

    if (n == 0) return 0;  /* behaviour below offset is irrelevant */

    while (terms_len < n) {
        seq_t cube = seq_pow((seq_t)next_k, (unsigned)3);

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
