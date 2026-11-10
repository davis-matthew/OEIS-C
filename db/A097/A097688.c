/*
 * A097688  Number of ones in the first n terms of A097686.
 * https://oeis.org/A097688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k : A097686(k)=1, 1<=k<=n}; A097686(1)=1, A097686(m+1)
 *            = A097686(m)/2 if even, else A097686(m) + #{earlier occurrences
 *            of A097686(m)}
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A097688_terms = NULL;
static size_t *A097688_ones = NULL;
static size_t A097688_cap = 0;
static size_t A097688_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (A097688_len == 0) {
        A097688_cap = 128;
        A097688_terms = mem_new(seq_t, A097688_cap + 1);
        A097688_ones = (size_t *)calloc(A097688_cap + 1, sizeof(size_t));
        if (!A097688_terms || !A097688_ones) return (seq_t)0;
        A097688_terms[1] = (seq_t)1;
        A097688_ones[1] = 1;
        A097688_len = 1;
    }

    if ((size_t)n > A097688_len) {
        if (A097688_cap <= (size_t)n) {
            size_t ncap = A097688_cap;
            while (ncap <= (size_t)n) ncap <<= 1;
            seq_t *t = mem_resize(seq_t, A097688_terms, ncap + 1);
            if (!t) return (seq_t)0;
            A097688_terms = t;
            size_t *o = (size_t *)realloc(A097688_ones, (ncap + 1) * sizeof(size_t));
            if (!o) return (seq_t)0;
            A097688_ones = o;
            A097688_cap = ncap;
        }
        for (size_t i = A097688_len + 1; i <= (size_t)n; ++i) {
            seq_t prev = A097688_terms[i - 1];
            seq_t next;
            if ((prev % (seq_t)2) == 0) {
                next = prev / (seq_t)2;
            } else {
                size_t earlier = 0;
                for (size_t j = 1; j + 1 < i; ++j)
                    if (A097688_terms[j] == prev) ++earlier;
                next = prev + (seq_t)earlier;
            }
            A097688_terms[i] = next;
            A097688_ones[i] = A097688_ones[i - 1] + (next == (seq_t)1 ? 1 : 0);
        }
        A097688_len = (size_t)n;
    }

    return (seq_t)A097688_ones[(size_t)n];
}
