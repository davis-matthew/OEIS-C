/*
 * A005525  Maximal number of rational points on a curve of genus 2 over
 *          GF(q), where q = A246655(n) is the n-th prime power > 1.
 * https://oeis.org/A005525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Serre's theorem. q = A246655(n), m = floor(2*sqrt(q)), p = the
 *            prime dividing q. q is "special" if (q is not a square and p |
 *            m) or q = x^2+1, x^2+x+1 or x^2+x+2. a(n) = q+1+2m if q is not
 *            special (exceptions: q=4 -> 10, q=9 -> 20); if q is special:
 *            a(n) = q+2m when frac(2*sqrt(q)) >= (sqrt(5)-1)...
 * Range:     u8 n <= 59, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isq64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isq64.h"

static uint64_t *pp_q = NULL;   /* prime powers in increasing order */
static uint64_t *pp_p = NULL;   /* their prime */
static uint64_t pp_cnt = 0;

static void build(uint64_t limit) {
    unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
    uint64_t *pr = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    uint64_t i, j;
    for (i = 0; i <= limit; i++) pr[i] = 0;
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
            for (j = i; j <= limit; j *= i) {
                pr[j] = i;
                if (j > limit / i) break;
            }
        }
    }
    free(pp_q); free(pp_p);
    pp_cnt = 0;
    for (i = 2; i <= limit; i++) if (pr[i]) pp_cnt++;
    pp_q = (uint64_t *)malloc((pp_cnt + 1) * sizeof(uint64_t));
    pp_p = (uint64_t *)malloc((pp_cnt + 1) * sizeof(uint64_t));
    j = 0;
    for (i = 2; i <= limit; i++) if (pr[i]) { pp_q[j] = i; pp_p[j] = pr[i]; j++; }
    free(comp); free(pr);
}

seq_t a(idx_t n) {
    uint64_t limit = 1024;
    while (pp_cnt < n) {
        while (limit / 8 < n + 16) limit *= 2;
        build(limit);
        limit *= 2;
    }
    uint64_t q = pp_q[n - 1], p = pp_p[n - 1];
    uint64_t res;
    if (q == 2) res = 6;
    else if (q == 3) res = 8;
    else if (q == 4) res = 10;
    else if (q == 9) res = 20;
    else {
        uint64_t m = isq64(4 * q);
        uint64_t s = isq64(q);
        bool square = (s * s == q);
        bool special = (!square && m % p == 0);
        uint64_t x = isq64(q - 1);
        if (x * x + 1 == q) special = true;
        uint64_t y;
        for (y = (s > 0 ? s - 1 : 0); y <= s + 1; y++) {
            if (y * y + y + 1 == q || y * y + y + 2 == q) special = true;
        }
        if (!special) res = q + 1 + 2 * m;
        else {
            /* frac(2 sqrt q) >= (sqrt5-1)/2  <=>  16q+5-(2m-1)^2 >= 8 sqrt(5q) */
            long long L = (long long)(16 * q + 5) - (long long)((2 * m - 1) * (2 * m - 1));
            bool big = (L >= 0) && ((unsigned long long)L * (unsigned long long)L >= 320ull * q);
            res = big ? q + 2 * m : q + 2 * m - 1;
        }
    }
    return (seq_t)res;
}
