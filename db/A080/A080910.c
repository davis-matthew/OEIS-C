/*
 * A080910  Number of numbers that differ from n in decimal representation by
 *          exactly one edit-operation: deletion, insertion, or substitution.
 * https://oeis.org/A080910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{i : LevenshteinDistance(n,i)=1}
 * Range:     u8 n <= 12, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: qsort compare for seq_t */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* returns the number of integers at Levenshtein distance exactly one
   from the decimal representation of n. */
seq_t a(idx_t n)
{
    const seq_t N = (seq_t)n;

    /* extract decimal digits of n (most significant first) */
    int digits[64];
    int len;
    if (N == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        int rev[64];
        int rev_len = 0;
        while (tmp > 0) {
            rev[rev_len++] = tmp % 10;
            tmp /= 10;
        }
        len = rev_len;
        for (int i = 0; i < rev_len; ++i)
            digits[i] = rev[rev_len - 1 - i];
    }

    /* buffer for candidate numbers */
    seq_t cand[256];
    size_t sz = 0;

    /* Substitution: replace each digit with 0–9 except itself */
    for (int i = 0; i < len; ++i) {
        int orig = digits[i];
        for (int d = 0; d <= 9; ++d) {
            if (d == orig) continue;
            /* skip substitution that would create a leading zero */
            if (i == 0 && d == 0 && len > 1) continue;
            seq_t val = 0;
            for (int j = 0; j < len; ++j)
                val = val * 10 + ((j == i) ? d : digits[j]);
            if (val == N) continue;
            cand[sz++] = val;
        }
    }

    /* Deletion: remove each digit */
    for (int i = 0; i < len; ++i) {
        if (len == 1) continue;          /* cannot delete the only digit */
        /* skip deletion that would leave a leading zero with more than
           one digit left (i.e., length > 2 after deletion) */
        if (i == 0 && len > 2 && digits[1] == 0) continue;
        seq_t val = 0;
        for (int j = 0; j < len; ++j) {
            if (j == i) continue;
            val = val * 10 + digits[j];
        }
        if (val == N) continue;
        cand[sz++] = val;
    }

    /* Insertion: insert 0–9 at every possible position */
    for (int pos = 0; pos <= len; ++pos) {
        for (int d = 0; d <= 9; ++d) {
            /* skip insertion that would produce a leading zero */
            if (pos == 0 && d == 0) continue;
            seq_t val = 0;
            for (int j = 0; j < pos; ++j) val = val * 10 + digits[j];
            val = val * 10 + d;
            for (int j = pos; j < len; ++j) val = val * 10 + digits[j];
            if (val == N) continue;
            cand[sz++] = val;
        }
    }

    /* Sort and count unique values */
    qsort(cand, sz, sizeof(seq_t), cmp_seq);
    seq_t result = 0;
    if (sz > 0) {
        result = 1;
        for (size_t i = 1; i < sz; ++i)
            if (cand[i] != cand[i - 1]) ++result;
    }
    return result;
}
