/*
 * A229132  Initials for A159559 corresponding to records of A229019.
 * https://oeis.org/A229132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the n-th record initial: for each prime p >= 3, let m(p)
 *            be the first index N >= 2 where the greedy sequence starting
 *            b(2)=p (b(k) = least value > b(k-1) that is prime iff k is
 *            prime) equals A159559(N); a(n) = the p giving the n-th strict
 *            record of m(p) (A229019). Computed by sieve + search
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t A229132_recs[64];
static idx_t A229132_cnt = 0;

/* returns merge index for initial p, 0 if not merged within N, (uint64_t)-1 if sieve too small */
static uint64_t A229132_merge(uint64_t p, const unsigned char *comp, const uint64_t *ref, uint64_t N, uint64_t L)
{
    uint64_t s = p;
    if (s == ref[2]) return 2;
    for (uint64_t n = 3; n <= N; n++) {
        bool pn = !comp[n];
        s++;
        for (;;) {
            if (s >= L) return (uint64_t)-1;
            bool ps = !comp[s];
            if (ps == pn) break;
            s++;
        }
        if (s == ref[n]) return n;
    }
    return 0;
}

static bool A229132_compute(idx_t k, uint64_t N, uint64_t L)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 1, 1);
    uint64_t *ref = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(N + 2));
    if (!comp || !ref) { free(comp); free(ref); return false; }
    comp[0] = comp[1] = 1;
    for (uint64_t i = 2; i * i <= L; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
    ref[2] = 3;
    for (uint64_t n = 3; n <= N; n++) {
        uint64_t s = ref[n - 1] + 1;
        bool pn = !comp[n];
        for (;;) {
            if (s >= L) { free(comp); free(ref); return false; }
            if ((!comp[s]) == pn) break;
            s++;
        }
        ref[n] = s;
    }
    idx_t cnt = 0;
    uint64_t best = 0;
    bool ok = true;
    for (uint64_t p = 3; p < L && cnt < k; p++) {
        if (comp[p]) continue;
        uint64_t m = A229132_merge(p, comp, ref, N, L);
        if (m == (uint64_t)-1 || m == 0) { ok = false; break; }
        if (m > best) {
            best = m;
            if (cnt < 64) A229132_recs[cnt] = p;
            cnt++;
        }
    }
    free(comp);
    free(ref);
    if (ok && cnt >= k) {
        A229132_cnt = cnt < 64 ? cnt : 64;
        return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > 64) return (seq_t)0;
    if (n > A229132_cnt) {
        uint64_t N = 1u << 12;
        uint64_t L = N * 32;
        for (;;) {
            if (A229132_compute(n, N, L)) break;
            N *= 2;
            L *= 2;
        }
    }
    return (seq_t)A229132_recs[n - 1];
}
