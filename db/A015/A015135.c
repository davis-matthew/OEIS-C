/*
 * A015135  Consider Fibonacci-type sequences f(0)=X, f(1)=Y,
 *          f(k)=f(k-1)+f(k-2) mod n; all are periodic; sequence gives number
 *          of distinct periods.
 * https://oeis.org/A015135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct cycle lengths of the Fibonacci
 *            recurrence modulo n
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t memo[10001];
static char memo_done[10001] = {0};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* off‑offset, not used by OEIS */

    /* cached value for n ≤ 10000 */
    if (n <= 10000 && memo_done[n]) return memo[n];

    size_t N = (size_t)n;
    size_t total = N * N;                      /* number of states (x,y) */
    size_t words = (total + 63) >> 6;          /* bits → 64‑bit words */
    size_t period_words = words + 1;           /* one extra word for length == total */

    /* bitsets for visited states and for observed cycle lengths */
    uint64_t *visited = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *period  = (uint64_t *)calloc(period_words, sizeof(uint64_t));
    if (!visited || !period) {
        free(visited);
        free(period);
        return 0;
    }

    size_t distinct = 0;

    for (size_t idx = 0; idx < total; ++idx) {
        /* already processed? */
        if (visited[idx >> 6] & (1ULL << (idx & 63))) continue;

        /* traverse cycle starting at idx */
        size_t cur = idx;
        size_t len = 0;
        do {
            /* mark current state */
            visited[cur >> 6] |= 1ULL << (cur & 63);

            /* compute next state: (x,y) -> (y, (x+y) mod N) */
            size_t x = cur / N;
            size_t y = cur % N;
            size_t r = x + y;
            if (r >= N) r -= N;
            cur = y * N + r;
            ++len;
        } while (cur != idx);   /* permutation guarantees return to start */

        /* record this cycle length */
        period[len >> 6] |= 1ULL << (len & 63);
    }

    /* count distinct lengths */
    for (size_t i = 0; i < period_words; ++i) {
        distinct += __builtin_popcountll(period[i]);
    }

    free(visited);
    free(period);

    /* memoize the result if small enough */
    if (n <= 10000) {
        memo_done[n] = 1;
        memo[n] = (seq_t)distinct;
    }
    return (seq_t)distinct;
}
