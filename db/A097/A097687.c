/*
 * A097687  Smallest m such that A097686(m)=n.
 * https://oeis.org/A097687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that A097686(m) = n, found by simulating
 *            A097686 (b(1)=1; b(k+1) = b(k)/2 if b(k) even, else b(k) +
 *            #earlier occurrences of b(k))
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A097686_t = NULL;
static size_t A097686_len = 0;
static size_t A097686_cap = 0;

static int A097686_extend(void)
{
    if (A097686_len == 0) {
        A097686_cap = 1024;
        A097686_t = mem_new(seq_t, A097686_cap);
        if (!A097686_t) return 0;
        A097686_t[0] = (seq_t)1;
        A097686_len = 1;
    }
    if (A097686_len >= A097686_cap) {
        size_t nc = A097686_cap * 2;
        seq_t *tmp = mem_resize(seq_t, A097686_t, nc);
        if (!tmp) return 0;
        A097686_t = tmp;
        A097686_cap = nc;
    }
    seq_t prev = A097686_t[A097686_len - 1];
    seq_t next;
    if ((prev % (seq_t)2) == 0) {
        next = prev / (seq_t)2;
    } else {
        size_t earlier = 0;
        for (size_t j = 0; j + 1 < A097686_len; ++j)
            if (A097686_t[j] == prev) ++earlier;
        next = prev + (seq_t)earlier;
    }
    A097686_t[A097686_len++] = next;
    return 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t target = (seq_t)n;
    size_t scanned = 0;
    if (A097686_len == 0 && !A097686_extend()) return 0;
    for (;;) {
        while (scanned < A097686_len) {
            if (A097686_t[scanned] == target) return (seq_t)(scanned + 1);
            ++scanned;
        }
        if (!A097686_extend()) return 0;
    }
}
