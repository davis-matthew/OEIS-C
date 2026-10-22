/*
 * A005580  a(n) = smallest number k such that Product_{i=2..k+1}
 *          prime(i)/(prime(i)-1) > n.
 * https://oeis.org/A005580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   search. Sieve the odd primes 3, 5, 7, ... in order and keep a
 *            running sum of log(p/(p-1)) in long double with Kahan summation.
 *            a(n) is the number of primes taken when the sum first exceeds
 *            log(n). The product is N/D with N odd and D even, so it is never
 *            exactly an integer and ties cannot happen
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SEG 262144
#define BASE_LIMIT 1048576
#define MAXRES 64

static uint32_t *bp = NULL;
static uint32_t nbp = 0;

static unsigned char seg[SEG];
static uint64_t seg_base = 3;
static uint32_t seg_pos = 0;
static bool seg_valid = false;

static long double lsum = 0.0L, lcomp = 0.0L;
static uint64_t cnt = 0;
static uint64_t res[MAXRES];
static int nres = 0;
static long double thr = 0.0L;

static void init_base(void) {
    unsigned char *s = (unsigned char *)calloc(BASE_LIMIT + 1, 1);
    uint32_t i, j;
    bp = (uint32_t *)malloc(sizeof(uint32_t) * 90000);
    for (i = 2; (uint64_t)i * i <= BASE_LIMIT; i++)
        if (!s[i])
            for (j = i * i; j <= BASE_LIMIT; j += i) s[j] = 1;
    for (i = 3; i <= BASE_LIMIT; i += 2)
        if (!s[i]) bp[nbp++] = i;
    free(s);
}

static void build_segment(void) {
    uint64_t hi = seg_base + 2 * (uint64_t)(SEG - 1);
    uint32_t k;
    memset(seg, 0, SEG);
    for (k = 0; k < nbp; k++) {
        uint64_t p = bp[k];
        uint64_t s = p * p;
        uint64_t j;
        if (s > hi) break;
        if (s < seg_base) {
            s = ((seg_base + p - 1) / p) * p;
            if ((s & 1) == 0) s += p;
        }
        for (j = (s - seg_base) / 2; j < SEG; j += p) seg[j] = 1;
    }
    seg_pos = 0;
    seg_valid = true;
}

static void add_prime(uint64_t p) {
    long double t, y, tt;
    if (p < 1000000) {
        t = log1pl(1.0L / (long double)(p - 1));
    } else {
        long double x = 1.0L / (long double)p;
        t = x * (1.0L + x * (0.5L + x * (1.0L / 3.0L + x * 0.25L)));
    }
    y = t - lcomp;
    tt = lsum + y;
    lcomp = (tt - lsum) - y;
    lsum = tt;
}

seq_t a(idx_t n) {
    if (bp == NULL) {
        init_base();
        res[0] = 0;
        nres = 1;
        thr = 0.0L; /* log(1) */
    }
    if (n >= MAXRES) return 0;
    while ((idx_t)nres <= n) {
        if (!seg_valid || seg_pos >= SEG) {
            if (seg_valid) seg_base += 2 * (uint64_t)SEG;
            build_segment();
        }
        while (seg_pos < SEG && (idx_t)nres <= n) {
            if (!seg[seg_pos]) {
                uint64_t p = seg_base + 2 * (uint64_t)seg_pos;
                add_prime(p);
                cnt++;
                while (nres < MAXRES && lsum > thr) {
                    res[nres++] = cnt;
                    thr = logl((long double)nres);
                }
            }
            seg_pos++;
        }
    }
    return (seq_t)res[n];
}
