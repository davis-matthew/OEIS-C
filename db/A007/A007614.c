/*
 * A007614  All values attained by the phi(n) function, in ascending order.
 * https://oeis.org/A007614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sieve. a(n) is the n-th smallest value in the multiset {phi(m) :
 *            m >= 1}. For each bound V, all m with phi(m) <= V lie below a
 *            limit set by the primorials: phi(m) >= m*phi(P_j)/P_j for P_j <=
 *            m < P_(j+1). Sieve phi up to that limit, count how often each
 *            value <= V occurs, and pick the n-th
 * Range:     u8 n <= 495, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 88 terms through n = 9292 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t phi_limit(uint64_t V) {
    static const uint64_t pr[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    uint64_t P = 1, ph = 1, M = 1;
    int i;
    for (i = 0; i < 15; i++) {
        uint64_t Pn, hi;
        if (ph > V) break;
        Pn = P * pr[i];
        hi = V * P / ph;
        if (hi > Pn - 1) hi = Pn - 1;
        if (hi > M) M = hi;
        P = Pn;
        ph *= pr[i] - 1;
    }
    return M;
}

static uint64_t *cum = NULL;   /* cum[v] = #{m : phi(m) <= v}, valid for v <= cumV */
static uint64_t cumV = 0;

static void build(uint64_t V) {
    uint64_t M = phi_limit(V), i, k, v;
    uint32_t *ph = (uint32_t *)malloc((size_t)(M + 1) * sizeof(uint32_t));
    uint64_t *c = (uint64_t *)calloc((size_t)(V + 1), sizeof(uint64_t));
    for (i = 0; i <= M; i++) ph[i] = (uint32_t)i;
    for (i = 2; i <= M; i++) {
        if (ph[i] == i) {
            for (k = i; k <= M; k += i) ph[k] -= ph[k] / (uint32_t)i;
        }
    }
    for (i = 1; i <= M; i++) {
        if (ph[i] <= V) c[ph[i]]++;
    }
    for (v = 1; v <= V; v++) c[v] += c[v - 1];
    free(ph);
    if (cum) free(cum);
    cum = c;
    cumV = V;
}

seq_t a(idx_t n) {
    uint64_t lo, hi;
    if (cum == NULL || cum[cumV] < (uint64_t)n) {
        uint64_t V = cumV * 2;
        if (V < (uint64_t)n) V = (uint64_t)n;
        if (V < 16) V = 16;
        build(V);
        while (cum[cumV] < (uint64_t)n) build(cumV * 2);
    }
    lo = 1;
    hi = cumV;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= (uint64_t)n) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)lo;
}
