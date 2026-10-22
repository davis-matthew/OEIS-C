/*
 * A003033  Smallest integer m such that the product of every 4 consecutive
 *          integers > m has a prime factor > prime(n).
 * https://oeis.org/A003033
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that k, k+1, k+2, k+3 all have greatest
 *            prime factor <= prime(n); found with a segmented
 *            greatest-prime-factor sieve over k <= 2^26, scanned downward to
 *            keep a staircase of (max gpf, k) records
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   a3033_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3033_nth_prime.h"

#define A3033_LIM 67108864u   /* numbers 1..LIM are sieved */
#define A3033_SEG 65536u

static int a3033_built = 0;
static uint32_t *a3033_fm = NULL;   /* max gpf of the 4-run, decreasing */
static uint32_t *a3033_fk = NULL;   /* k, decreasing */
static size_t a3033_fn = 0, a3033_fcap = 0;

static void a3033_push(uint32_t m, uint32_t k) {
    if (a3033_fn == a3033_fcap) {
        size_t nc = a3033_fcap ? 2 * a3033_fcap : 256;
        a3033_fm = (uint32_t *)realloc(a3033_fm, nc * sizeof(uint32_t));
        a3033_fk = (uint32_t *)realloc(a3033_fk, nc * sizeof(uint32_t));
        a3033_fcap = nc;
    }
    a3033_fm[a3033_fn] = m;
    a3033_fk[a3033_fn] = k;
    a3033_fn++;
}

static void a3033_build(void) {
    uint32_t sq = 1;
    while ((uint64_t)(sq + 1) * (sq + 1) <= A3033_LIM) sq++;
    bool *comp = (bool *)calloc(sq + 1, sizeof(bool));
    uint32_t *pr = (uint32_t *)malloc((sq + 1) * sizeof(uint32_t));
    uint32_t np = 0;
    for (uint32_t i = 2; i <= sq; i++) {
        if (!comp[i]) {
            pr[np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= sq; j += i) comp[j] = true;
        }
    }
    free(comp);

    uint32_t *rem = (uint32_t *)malloc(A3033_SEG * sizeof(uint32_t));
    uint32_t *gp = (uint32_t *)malloc(A3033_SEG * sizeof(uint32_t));
    uint32_t carry[3];
    int ncarry = 0;
    uint32_t curmin = UINT32_MAX;
    uint64_t hi = (uint64_t)A3033_LIM + 1;   /* segment is [lo, hi) */
    while (hi > 1) {
        uint64_t lo = hi > (uint64_t)A3033_SEG + 1 ? hi - A3033_SEG : 1;
        uint32_t len = (uint32_t)(hi - lo);
        for (uint32_t i = 0; i < len; i++) { rem[i] = 1; gp[i] = 1; }
        for (uint32_t t = 0; t < np; t++) {
            uint64_t p = pr[t];
            if (p * p > hi - 1) break;
            for (uint64_t pk = p; pk <= hi - 1; pk *= p) {
                uint64_t s = ((lo + pk - 1) / pk) * pk;
                for (uint64_t j = s; j < hi; j += pk) {
                    rem[j - lo] *= (uint32_t)p;
                    gp[j - lo] = (uint32_t)p;
                }
            }
        }
        for (uint32_t i = 0; i < len; i++) {
            uint32_t c = (uint32_t)((lo + i) / rem[i]);
            if (c > 1) gp[i] = c;
        }
        for (uint32_t ii = len; ii-- > 0;) {
            if (ii + 3 >= len && (int)(ii + 3 - len) >= ncarry) continue;
            uint32_t m = 0;
            for (uint32_t t = 0; t < 4; t++) {
                uint32_t idx = ii + t;
                uint32_t g = idx < len ? gp[idx] : carry[idx - len];
                if (g > m) m = g;
            }
            if (m < curmin) {
                curmin = m;
                a3033_push(m, (uint32_t)(lo + ii));
            }
        }
        for (int t = 0; t < 3; t++) carry[t] = gp[t];
        ncarry = 3;
        hi = lo;
    }
    free(rem);
    free(gp);
    free(pr);
    a3033_built = 1;
}

seq_t a(idx_t n) {
    if (!a3033_built) a3033_build();
    uint64_t p = a3033_nth_prime(n);
    for (size_t i = 0; i < a3033_fn; i++) {
        if ((uint64_t)a3033_fm[i] <= p) return (seq_t)a3033_fk[i];
    }
    return (seq_t)0;
}
