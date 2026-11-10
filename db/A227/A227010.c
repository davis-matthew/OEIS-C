/*
 * A227010  Places n where A225867(n) = 4.
 * https://oeis.org/A227010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m such that A225867(m) = 4; found by search, with
 *            A225867(m) = max_{2<=k<=(m-2)/2} #{d>1: d|m+k, k|m+d} computed
 *            by enumerating d and k = -m mod d
 * Range:     u8 n <= 48, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int *A227010_cnt = 0;
static size_t A227010_cap = 0;

static int A227010_val(uint64_t n)
{
    if (n < 6) return 0;
    uint64_t max_k = (n - 2) / 2;
    if (A227010_cap < max_k + 1) {
        A227010_cap = (size_t)(max_k + 1) * 2;
        A227010_cnt = (int *)realloc(A227010_cnt, A227010_cap * sizeof(int));
    }
    memset(A227010_cnt, 0, (size_t)(max_k + 1) * sizeof(int));
    uint64_t dmax = n + max_k;
    for (uint64_t d = 2; d <= dmax; ++d) {
        uint64_t k = (d - n % d) % d;   /* k = -n mod d */
        while (k < 2) k += d;
        for (; k <= max_k; k += d) {
            if ((n + d) % k == 0) A227010_cnt[k]++;
        }
    }
    int best = 0;
    for (uint64_t k = 2; k <= max_k; ++k)
        if (A227010_cnt[k] > best) best = A227010_cnt[k];
    return best;
}

static uint64_t *A227010_terms = 0;
static size_t A227010_nt = 0, A227010_tcap = 0;
static uint64_t A227010_next = 6;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A227010_nt < (size_t)n) {
        uint64_t m = A227010_next++;
        if (A227010_val(m) == 4) {
            if (A227010_nt == A227010_tcap) {
                A227010_tcap = A227010_tcap ? A227010_tcap * 2 : 256;
                A227010_terms = (uint64_t *)realloc(A227010_terms, A227010_tcap * sizeof(uint64_t));
            }
            A227010_terms[A227010_nt++] = m;
        }
    }
    return (seq_t)A227010_terms[n - 1];
}
