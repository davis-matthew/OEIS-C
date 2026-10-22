/*
 * A001276  Smallest k such that the product of q/(q-1) over the primes from
 *          prime(n) to prime(n+k-1) is greater than 2.
 * https://oeis.org/A001276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- segmented sieve over the primes from prime(n) upward,
 *            summing log(q/(q-1)) until the sum exceeds log(2); a(n) = number
 *            of primes used
 * Range:     u8 n <= 12, u16 n <= 152, u32 n <= 650, u64 n <= 650,
 *            u128 n <= 650, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static uint32_t *a001276_bp = NULL;
static size_t a001276_nbp = 0;
static uint64_t a001276_bplim = 0;

static void a001276_grow_base(uint64_t lim) {
    if (lim <= a001276_bplim) return;
    unsigned char *s = (unsigned char *)calloc((size_t)lim + 1, 1);
    size_t cnt = 0;
    for (uint64_t i = 3; i <= lim; i += 2) {
        if (!s[i]) {
            cnt++;
            if (i * i <= lim)
                for (uint64_t j = i * i; j <= lim; j += 2 * i) s[j] = 1;
        }
    }
    a001276_bp = (uint32_t *)realloc(a001276_bp, (cnt ? cnt : 1) * sizeof(uint32_t));
    size_t k = 0;
    for (uint64_t i = 3; i <= lim; i += 2)
        if (!s[i]) a001276_bp[k++] = (uint32_t)i;
    free(s);
    a001276_nbp = cnt;
    a001276_bplim = lim;
}

#define A001276_SEG 262144

seq_t a(idx_t n) {
    static unsigned char seg[A001276_SEG];
    const double L = 0.69314718055994530942;
    if (n == 1) return (seq_t)2;

    uint64_t *nx = NULL;
    size_t nxcap = 0, na = 0;
    idx_t cnt = 1;              /* prime 2 already counted */
    uint64_t k = 0;
    double sum = 0.0, comp = 0.0;

    for (uint64_t lo = 3;; lo += 2 * (uint64_t)A001276_SEG) {
        uint64_t hi = lo + 2 * (uint64_t)A001276_SEG;
        for (;;) {
            if (na < a001276_nbp) {
                uint64_t q = a001276_bp[na];
                if (q * q < hi) {
                    if (na >= nxcap) {
                        nxcap = a001276_nbp;
                        nx = (uint64_t *)realloc(nx, nxcap * sizeof(uint64_t));
                    }
                    nx[na] = q * q;
                    na++;
                    continue;
                }
                break;
            } else {
                a001276_grow_base(a001276_bplim ? a001276_bplim * 2 : 65536);
            }
        }
        memset(seg, 0, sizeof(seg));
        for (size_t i = 0; i < na; i++) {
            uint64_t q2 = 2 * (uint64_t)a001276_bp[i];
            uint64_t j = nx[i];
            for (; j < hi; j += q2) seg[(j - lo) >> 1] = 1;
            nx[i] = j;
        }
        for (size_t i = 0; i < A001276_SEG; i++) {
            if (seg[i]) continue;
            uint64_t p = lo + 2 * (uint64_t)i;
            cnt++;
            if (cnt < n) continue;
            k++;
            double x = 1.0 / (double)p;
            double t;
            if (p < 1000) t = -log1p(-x);
            else t = x * (1.0 + x * (0.5 + x * (1.0 / 3.0 + x * 0.25)));
            double y = t - comp;
            double tt = sum + y;
            comp = (tt - sum) - y;
            sum = tt;
            if (sum > L) {
                free(nx);
                return (seq_t)k;
            }
        }
    }
}
