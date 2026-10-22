/*
 * A007936  Some permutation of digits is a square.
 * https://oeis.org/A007936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search: m (taken in order) is a term iff the multiset of its L
 *            digits equals the digits of some square k^2 < 10^L, padded with
 *            leading zeros to L digits
 * Range:     u8 n <= 39, u16 n <= 6948, u32 n <= 6948, u64 n <= 6948,
 *            u128 n <= 6948, big any n
 * Helpers:   digsig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digsig.h"

static uint64_t *terms = NULL;
static idx_t tcnt = 0, tcap = 0;
static int curL = 0;
static uint64_t *sqsig = NULL;
static size_t nsq = 0;
static uint64_t nextm = 1, hiL = 1;

static int cmp_u64(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static int has_sig(uint64_t s) {
    size_t lo = 0, hi = nsq;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (sqsig[mid] < s) lo = mid + 1;
        else hi = mid;
    }
    return lo < nsq && sqsig[lo] == s;
}

static void next_length(void) {
    uint64_t lo = hiL, k, cnt = 0;
    curL++;
    hiL = lo * 10;
    for (k = 1; k * k < hiL; k++) cnt++;
    free(sqsig);
    sqsig = (uint64_t *)malloc((cnt ? cnt : 1) * sizeof(uint64_t));
    nsq = 0;
    for (k = 1; k * k < hiL; k++) sqsig[nsq++] = digsig(k * k, curL);
    qsort(sqsig, nsq, sizeof(uint64_t), cmp_u64);
    nextm = (curL == 1) ? 1 : lo;
}

seq_t a(idx_t n) {
    while (tcnt < n) {
        if (curL == 0 || nextm >= hiL) {
            next_length();
            continue;
        }
        if (has_sig(digsig(nextm, curL))) {
            if (tcnt == tcap) {
                tcap = tcap ? tcap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, tcap * sizeof(uint64_t));
            }
            terms[tcnt++] = nextm;
        }
        nextm++;
    }
    return (seq_t)(idx_t)terms[n - 1];
}
