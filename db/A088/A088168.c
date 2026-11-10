/*
 * A088168  Smallest number m such that A088167(m)=n.
 * https://oeis.org/A088168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- sieve A088167 (a(1)=1, a(n) = #{k<n : A088167(k) | n})
 *            over a doubling range and record the first index m where
 *            A088167(m) = n
 * Range:     u8 n <= 19, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *A088168_first = NULL;
static uint64_t A088168_L = 0;

static void A088168_build(uint64_t L)
{
    uint32_t *cnt = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint32_t *fst = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    if (!cnt || !fst) { free(cnt); free(fst); return; }
    for (uint64_t i = 1; i <= L; i++) {
        uint64_t v = (i == 1) ? 1 : cnt[i];
        if (v >= 1 && v <= L) {
            if (fst[v] == 0) fst[v] = (uint32_t)i;
            /* every multiple j of v with j > i gets one more earlier divisor */
            for (uint64_t j = (i / v + 1) * v; j <= L; j += v) cnt[j]++;
        }
    }
    free(cnt);
    free(A088168_first);
    A088168_first = fst;
    A088168_L = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    for (;;) {
        if (A088168_first && (uint64_t)n <= A088168_L && A088168_first[(size_t)n] != 0)
            return (seq_t)A088168_first[(size_t)n];
        uint64_t L = A088168_L ? A088168_L * 2 : 4096;
        if (L > ((uint64_t)1 << 31)) return (seq_t)0;
        A088168_build(L);
        if (!A088168_first) return (seq_t)0;
    }
}
