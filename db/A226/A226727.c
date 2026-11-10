/*
 * A226727  Places n where A225867(n)=5.
 * https://oeis.org/A226727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m (m>=6) with A225867(m) = 5; found by scan, with
 *            A225867 computed by sieving multiples of k
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t A226727_f(idx_t n)
{
    if (n < 6) return 0;
    idx_t max_k = (n - 2) / 2;
    idx_t best = 0;
    for (idx_t k = 2; k <= max_k; ++k) {
        idx_t cnt = 0;
        idx_t lo = n + 2;
        idx_t m = ((lo + k - 1) / k) * k;
        idx_t hi = 2 * n + k;
        idx_t t = n + k;
        for (; m <= hi; m += k) {
            idx_t d = m - n;
            if (t % d == 0) ++cnt;
        }
        if (cnt > best) best = cnt;
    }
    return best;
}

static idx_t *A226727_terms = 0;
static size_t A226727_cnt = 0, A226727_cap = 0;
static idx_t A226727_next = 6;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A226727_cnt < (size_t)n) {
        idx_t v = A226727_next++;
        if (A226727_f(v) == 5) {
            if (A226727_cnt == A226727_cap) {
                A226727_cap = A226727_cap ? A226727_cap * 2 : 256;
                A226727_terms = (idx_t *)realloc(A226727_terms, A226727_cap * sizeof(idx_t));
            }
            A226727_terms[A226727_cnt++] = v;
        }
    }
    return (seq_t)A226727_terms[n - 1];
}
