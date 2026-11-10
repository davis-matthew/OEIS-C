/*
 * A087830  Consecutive repeats in A087831, the triple recursion skip two
 *          Hofstadter Q of the second sort.
 * https://oeis.org/A087830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   let b = A087831, b(n) = b(|n-b(n-1)|) + b(b(|n-b(n-4)|)) for
 *            n>4, b(0..4) = 1. a(n) is the n-th smallest distinct value v for
 *            which some k has b(k) = b(k-1) = v. The code computes b,
 *            collects these values, sorts them and uses a value only when
 *            every term in the later half of the computed range is...
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint64_t *A087830_b = NULL;
static idx_t A087830_blen = 0, A087830_bcap = 0;

static idx_t A087830_absd(idx_t n, uint64_t v)
{
    return n >= v ? n - v : v - n;
}

static void A087830_extend(idx_t len)
{
    if (len <= A087830_blen) return;
    if (len > A087830_bcap) {
        idx_t nc = A087830_bcap ? A087830_bcap : 64;
        while (nc < len) nc *= 2;
        uint64_t *p = (uint64_t *)realloc(A087830_b, (size_t)nc * sizeof(uint64_t));
        if (!p) abort();
        A087830_b = p;
        A087830_bcap = nc;
    }
    for (idx_t i = A087830_blen; i < len; ++i) {
        if (i <= 4) {
            A087830_b[i] = 1;
        } else {
            uint64_t t1 = A087830_b[A087830_absd(i, A087830_b[i - 1])];
            idx_t d2 = A087830_absd(i, A087830_b[i - 4]);
            uint64_t t2 = A087830_b[A087830_b[d2]];
            A087830_b[i] = t1 + t2;
        }
    }
    A087830_blen = len;
}

seq_t a(idx_t n)
{
    idx_t L = 64;
    for (;;) {
        A087830_extend(L);
        /* collect distinct repeated values, sorted */
        uint64_t *vals = (uint64_t *)malloc((size_t)L * sizeof(uint64_t));
        if (!vals) abort();
        idx_t cnt = 0;
        for (idx_t i = 1; i < L; ++i) {
            if (A087830_b[i] == A087830_b[i - 1]) {
                uint64_t v = A087830_b[i];
                /* insertion with dedupe */
                idx_t pos = cnt;
                int dup = 0;
                while (pos > 0 && vals[pos - 1] >= v) {
                    if (vals[pos - 1] == v) { dup = 1; break; }
                    --pos;
                }
                if (dup) continue;
                for (idx_t j = cnt; j > pos; --j) vals[j] = vals[j - 1];
                vals[pos] = v;
                ++cnt;
            }
        }
        if (n < cnt) {
            uint64_t v = vals[n];
            int ok = 1;
            for (idx_t j = L / 2; j < L; ++j) {
                if (A087830_b[j] <= v) { ok = 0; break; }
            }
            if (ok) {
                free(vals);
                return (seq_t)v;
            }
        }
        free(vals);
        L *= 2;
    }
}
