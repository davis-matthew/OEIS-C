/*
 * A007684  Prime(n)*...*prime(a(n)) is the least product of consecutive
 *          primes that is non-deficient.
 * https://oeis.org/A007684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the least x such that
 *            Product_{k=n..x} (1 + 1/prime(k)) >= 2, i.e. Sum_{k=n..x} log(1
 *            + 1/prime(k)) >= log(2). The primes come from a segmented sieve
 *            that is extended as needed. Log sums are compensated (Kahan)
 *            long doubles. They are kept as checkpoints at each...
 * Range:     u8 n <= 12, u16 n <= 151, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SEG (1u << 20)
#define BLIM (1u << 20)

typedef struct { uint64_t cnt; long double s, c; } st_t;

static uint32_t *bp = NULL;
static size_t nbp = 0;
static unsigned char segbuf[SEG / 2];
static st_t *ck = NULL;
static size_t nck = 0, capck = 0;

static void init_base(void) {
    unsigned char *f = (unsigned char *)malloc(BLIM + 1);
    size_t i, j, cnt = 0;
    memset(f, 1, BLIM + 1);
    f[0] = f[1] = 0;
    for (i = 2; i * i <= BLIM; i++)
        if (f[i])
            for (j = i * i; j <= BLIM; j += i) f[j] = 0;
    for (i = 2; i <= BLIM; i++) if (f[i]) cnt++;
    bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    for (i = 2; i <= BLIM; i++) if (f[i]) bp[nbp++] = (uint32_t)i;
    free(f);
    capck = 1024;
    ck = (st_t *)malloc(capck * sizeof(st_t));
    ck[0].cnt = 0; ck[0].s = 0.0L; ck[0].c = 0.0L;
    nck = 1;
}

static long double val(const st_t *st) { return st->s - st->c; }

static void addp(st_t *st, uint64_t p) {
    long double x = 1.0L / (long double)p, v, y, t;
    if (p < 1000000) v = log1pl(x);
    else v = x * (1.0L - x * (0.5L - x * (1.0L / 3.0L - x * 0.25L)));
    y = v - st->c;
    t = st->s + y;
    st->c = (t - st->s) - y;
    st->s = t;
    st->cnt++;
}

/* mode 0: stop when cnt == tc; mode 1: stop when val >= T; mode 2: never */
static int stopped(const st_t *st, int mode, uint64_t tc, long double T) {
    if (mode == 0) return st->cnt == tc;
    if (mode == 1) return val(st) >= T;
    return 0;
}

static int process(uint64_t k, st_t *st, int mode, uint64_t tc, long double T) {
    uint64_t lo = k * (uint64_t)SEG, hi = lo + SEG;
    size_t i;
    if (stopped(st, mode, tc, T)) return 1;
    if (k == 0) {
        addp(st, 2);
        if (stopped(st, mode, tc, T)) return 1;
    }
    memset(segbuf, 1, sizeof(segbuf));
    if (k == 0) segbuf[0] = 0; /* number 1 */
    for (i = 1; i < nbp; i++) {
        uint64_t q = bp[i], start;
        if (q * q >= hi) break;
        start = ((lo + q - 1) / q) * q;
        if (start < q * q) start = q * q;
        if ((start & 1) == 0) start += q;
        for (; start < hi; start += 2 * q) segbuf[(start - lo - 1) / 2] = 0;
    }
    for (i = 0; i < SEG / 2; i++) {
        if (segbuf[i]) {
            addp(st, lo + 2 * i + 1);
            if (stopped(st, mode, tc, T)) return 1;
        }
    }
    return 0;
}

static void extend(void) {
    st_t st = ck[nck - 1];
    process(nck - 1, &st, 2, 0, 0.0L);
    if (nck == capck) {
        capck *= 2;
        ck = (st_t *)realloc(ck, capck * sizeof(st_t));
    }
    ck[nck++] = st;
}

seq_t a(idx_t n) {
    const long double LN2 = 0.693147180559945309417232121458176568L;
    uint64_t tc = (uint64_t)n - 1;
    size_t lo, hi;
    st_t st;
    long double T;

    if (bp == NULL) init_base();

    /* state after the first n-1 primes */
    while (ck[nck - 1].cnt <= tc) extend();
    lo = 0; hi = nck - 1; /* largest k with ck[k].cnt <= tc */
    while (lo < hi) {
        size_t mid = (lo + hi + 1) / 2;
        if (ck[mid].cnt <= tc) lo = mid; else hi = mid - 1;
    }
    st = ck[lo];
    process(lo, &st, 0, tc, 0.0L);

    /* tolerance handles the exact tie product == 2 (only at n = 1) */
    T = val(&st) + LN2 - 1e-17L;

    while (val(&ck[nck - 1]) < T) extend();
    lo = 0; hi = nck - 1; /* largest k with val(ck[k]) < T */
    while (lo < hi) {
        size_t mid = (lo + hi + 1) / 2;
        if (val(&ck[mid]) < T) lo = mid; else hi = mid - 1;
    }
    st = ck[lo];
    process(lo, &st, 1, 0, T);
    return (seq_t)st.cnt;
}
