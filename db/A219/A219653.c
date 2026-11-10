/*
 * A219653  Least inverse of A219652; a(n) = minimal i such that A219652(i) =
 *          n.
 * https://oeis.org/A219653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ i : A219652(i) = n }, where A219652(0)=0 and
 *            A219652(i) = 1 + A219652(i - s(i)), s = digit sum in factorial
 *            base; computed by incremental scan with memoized A219652 values
 * Range:     u8 n <= 53, u16 n <= 5155, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 74 terms through n = 9783 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A219653_UNSET ((idx_t)~(idx_t)0)

static uint32_t *A219653_f = NULL;
static idx_t *A219653_first = NULL;
static idx_t A219653_cnt = 0, A219653_cap = 0;

static idx_t A219653_fds(idx_t v)
{
    idx_t sum = 0, i = 1;
    while (v > 0) {
        idx_t div = i + 1;
        sum += v % div;
        v /= div;
        ++i;
    }
    return sum;
}

static void A219653_grow(void)
{
    idx_t oldcap = A219653_cap;
    idx_t newcap = oldcap ? oldcap * 2 : 1024;
    A219653_f = (uint32_t *)realloc(A219653_f, (size_t)newcap * sizeof(uint32_t));
    A219653_first = (idx_t *)realloc(A219653_first, (size_t)newcap * sizeof(idx_t));
    for (idx_t k = oldcap; k < newcap; k++) A219653_first[k] = A219653_UNSET;
    A219653_cap = newcap;
}

seq_t a(idx_t n)
{
    for (;;) {
        if (n < A219653_cap && A219653_first[n] != A219653_UNSET)
            return (seq_t)A219653_first[n];
        idx_t i = A219653_cnt;
        if (i >= A219653_cap) A219653_grow();
        uint32_t f = 0;
        if (i > 0) f = 1 + A219653_f[i - A219653_fds(i)];
        A219653_f[i] = f;
        if (A219653_first[f] == A219653_UNSET) A219653_first[f] = i;
        A219653_cnt++;
    }
}
