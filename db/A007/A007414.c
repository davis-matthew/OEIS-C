/*
 * A007414  Largest number not a sum of distinct primes >= prime(n).
 * https://oeis.org/A007414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest number not a sum of distinct primes >= prime(n);
 *            computed by a bitset subset-sum sieve over primes >= prime(n),
 *            stopped once a run of representable numbers [x,y] has length >=
 *            the next prime (then every number >= x is representable)
 * Range:     u8 n <= 20, u16 n <= 2445, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *sv = NULL;
static uint64_t svlim = 0;
static uint64_t *pr = NULL;
static uint64_t npr = 0;

static void ensure_sieve(uint64_t lim) {
    uint64_t i, j;
    if (lim <= svlim) return;
    if (lim < 2 * svlim) lim = 2 * svlim;
    free(sv);
    free(pr);
    sv = (unsigned char *)malloc(lim + 1);
    memset(sv, 1, lim + 1);
    sv[0] = 0;
    if (lim >= 1) sv[1] = 0;
    for (i = 2; i * i <= lim; i++)
        if (sv[i])
            for (j = i * i; j <= lim; j += i) sv[j] = 0;
    npr = 0;
    for (i = 2; i <= lim; i++) if (sv[i]) npr++;
    pr = (uint64_t *)malloc((npr + 1) * sizeof(uint64_t));
    npr = 0;
    for (i = 2; i <= lim; i++) if (sv[i]) pr[npr++] = i;
    svlim = lim;
}

static void shift_or(uint64_t *B, uint64_t W, uint64_t q) {
    uint64_t s = q / 64, b = q % 64, w;
    if (s >= W) return;
    for (w = W - 1;; w--) {
        uint64_t v;
        if (b == 0) {
            v = B[w - s];
        } else {
            v = B[w - s] << b;
            if (w > s) v |= B[w - s - 1] >> (64 - b);
        }
        B[w] |= v;
        if (w == s) break;
    }
}

#define GETB(B, t) (((B)[(t) >> 6] >> ((t) & 63)) & 1)

/* Return start of first run of set bits in [0,M] with length >= len, or 0 if none. */
static uint64_t find_run(const uint64_t *B, uint64_t M, uint64_t len) {
    uint64_t run = 0, t = 0;
    while (t <= M) {
        if ((t & 63) == 0 && t + 63 <= M && B[t >> 6] == ~(uint64_t)0) {
            run += 64;
            t += 64;
            if (run >= len) return t - run;
            continue;
        }
        if (GETB(B, t)) {
            run++;
            if (run >= len) return t + 1 - run;
        } else {
            run = 0;
        }
        t++;
    }
    return 0;
}

seq_t a(idx_t n) {
    uint64_t p, M;
    while (npr < n) ensure_sieve(2 * svlim + 100);
    p = pr[n - 1];
    M = 5 * p + 200;
    for (;;) {
        uint64_t W = M / 64 + 1, i, cnt = 0, X = 0;
        uint64_t *B;
        ensure_sieve(2 * M + 10);
        B = (uint64_t *)calloc(W, sizeof(uint64_t));
        B[0] = 1;
        for (i = n - 1; i < npr && pr[i] <= M; i++) {
            shift_or(B, W, pr[i]);
            cnt++;
            if ((cnt & 31) == 0 || pr[i + 1] > M) {
                X = find_run(B, M, pr[i + 1]);
                if (X > 0) { i++; break; }
            }
        }
        if (X > 0) {
            int64_t t;
            /* primes <= X can still represent numbers below X */
            for (; i < npr && pr[i] <= X; i++) shift_or(B, W, pr[i]);
            for (t = (int64_t)X - 1; t >= 0; t--) {
                if (!GETB(B, (uint64_t)t)) {
                    free(B);
                    return (seq_t)(uint64_t)t;
                }
            }
            free(B);
            return 0;
        }
        free(B);
        M *= 2;
    }
}
