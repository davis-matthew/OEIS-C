/*
 * A007077  Optimal cost of search tree for searching an ordered array of n
 *          elements with cost k of probing element k.
 * https://oeis.org/A007077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = m(n, 0) where m(0, d) = 0; for n > 0, m(n, d) = min_{1<=r<=n} {(r+d)*n + m(r-1, d) + m(n-r, r+d)}. Memoized recursion. */

#include <stdlib.h>

typedef struct {
    idx_t n;
    idx_t d;
    seq_t value;
} MemoEntry;

static MemoEntry *memo = NULL;
static idx_t memo_size = 0;
static idx_t memo_cap = 0;

static seq_t m_impl(idx_t n, idx_t d);

static seq_t m_impl(idx_t n, idx_t d) {
    if (n == 0) {
        return 0;
    }
    
    for (idx_t i = 0; i < memo_size; i++) {
        if (memo[i].n == n && memo[i].d == d) {
            return memo[i].value;
        }
    }
    
    seq_t n_seq = (seq_t)n;
    seq_t result = 0;
    for (idx_t r = 1; r <= n; r++) {
        seq_t r_seq = (seq_t)r;
        seq_t d_seq = (seq_t)d;
        seq_t cost = (r_seq + d_seq) * n_seq + m_impl(r - 1, d) + m_impl(n - r, r + d);
        if (r == 1 || cost < result) {
            result = cost;
        }
    }
    
    if (memo_size >= memo_cap) {
        memo_cap = memo_cap == 0 ? 16 : memo_cap * 2;
        memo = (MemoEntry *)realloc(memo, memo_cap * sizeof(MemoEntry));
    }
    memo[memo_size].n = n;
    memo[memo_size].d = d;
    memo[memo_size].value = result;
    memo_size++;
    
    return result;
}

seq_t a(idx_t n) {
    return m_impl(n, 0);
}
