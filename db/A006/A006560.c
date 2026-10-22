/*
 * A006560  Smallest starting prime for n consecutive primes in arithmetic
 *          progression.
 * https://oeis.org/A006560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: segmented sieve over the primes in increasing order, tracking runs of
   consecutive primes with equal gaps; a(n) is the first prime of the earliest run
   reaching length n (a(1) = 2). */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define A6560_SEG (1u << 18)
#define A6560_BASE 1000000u
#define A6560_LIMIT 1000000000000ULL

static uint64_t a6560_best[40];
static bool a6560_inited = false;
static uint32_t *a6560_bp = NULL;
static size_t a6560_nbp = 0;
static uint64_t a6560_lo = 3;
static uint64_t a6560_last = 2, a6560_gap = 0, a6560_rstart = 2;
static unsigned a6560_rlen = 1;
static unsigned char a6560_buf[A6560_SEG];

static void a6560_init(void) {
    unsigned char *s = (unsigned char *)malloc(A6560_BASE + 1);
    uint32_t i, j;
    size_t cnt = 0;
    memset(s, 1, A6560_BASE + 1);
    s[0] = s[1] = 0;
    for (i = 2; (uint64_t)i * i <= A6560_BASE; i++)
        if (s[i])
            for (j = i * i; j <= A6560_BASE; j += i) s[j] = 0;
    for (i = 3; i <= A6560_BASE; i++) if (s[i]) cnt++;
    a6560_bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    for (i = 3; i <= A6560_BASE; i++) if (s[i]) a6560_bp[a6560_nbp++] = i;
    free(s);
    memset(a6560_best, 0, sizeof(a6560_best));
    a6560_best[1] = 2;
    a6560_inited = true;
}

static void a6560_prime(uint64_t q) {
    uint64_t g = q - a6560_last;
    if (g == a6560_gap) {
        a6560_rlen++;
    } else {
        a6560_rstart = a6560_last;
        a6560_rlen = 2;
        a6560_gap = g;
    }
    a6560_last = q;
    if (a6560_rlen < 40 && a6560_best[a6560_rlen] == 0)
        a6560_best[a6560_rlen] = a6560_rstart;
}

static void a6560_segment(void) {
    uint64_t lo = a6560_lo;
    uint64_t hi = lo + 2ULL * A6560_SEG; /* covers odd numbers lo .. hi-2 */
    size_t k;
    uint32_t j;
    memset(a6560_buf, 1, A6560_SEG);
    for (k = 0; k < a6560_nbp; k++) {
        uint64_t p = a6560_bp[k];
        uint64_t st, idx;
        if (p * p >= hi) break;
        st = ((lo + p - 1) / p) * p;
        if (st < p * p) st = p * p;
        if ((st & 1) == 0) st += p;
        for (idx = (st - lo) / 2; idx < A6560_SEG; idx += p) a6560_buf[idx] = 0;
    }
    for (j = 0; j < A6560_SEG; j++)
        if (a6560_buf[j]) a6560_prime(lo + 2ULL * j);
    a6560_lo = hi;
}

seq_t a(idx_t n) {
    if (n < 1 || n >= 40) return 0;
    if (!a6560_inited) a6560_init();
    while (a6560_best[n] == 0 && a6560_lo < A6560_LIMIT) a6560_segment();
    return (seq_t)a6560_best[n];
}
