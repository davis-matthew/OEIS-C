/*
 * A007087  Next term is uniquely the sum of 3 earlier terms.
 * https://oeis.org/A007087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(1)=1, a(2)=2, a(3)=4; then a(n) is the smallest m >
 *            a(n-1) that is the sum of 3 distinct earlier terms in exactly
 *            one way. The code keeps counts of the pair sums and triple sums
 *            of earlier terms, capped at 2
 * Range:     u8 n <= 26, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static long long *a7087_terms = NULL;
static size_t a7087_cnt = 0, a7087_cap = 0;
static unsigned char *a7087_r2 = NULL, *a7087_r3 = NULL;
static size_t a7087_sz = 0;

static void a7087_ensure(size_t need) {
    size_t nsz;
    if (need <= a7087_sz) return;
    nsz = a7087_sz ? a7087_sz : 1024;
    while (nsz < need) nsz *= 2;
    a7087_r2 = (unsigned char *)realloc(a7087_r2, nsz);
    a7087_r3 = (unsigned char *)realloc(a7087_r3, nsz);
    memset(a7087_r2 + a7087_sz, 0, nsz - a7087_sz);
    memset(a7087_r3 + a7087_sz, 0, nsz - a7087_sz);
    a7087_sz = nsz;
}

static void a7087_add(long long t) {
    size_t i, x, lim;
    unsigned char *r2, *r3;
    a7087_ensure((size_t)(3 * t + 8));
    r2 = a7087_r2;
    r3 = a7087_r3;
    /* new triples: t plus a pair of earlier distinct terms */
    lim = (size_t)(2 * t);
    for (x = 0; x <= lim; x++) {
        unsigned v = (unsigned)r3[x + (size_t)t] + (unsigned)r2[x];
        r3[x + (size_t)t] = (unsigned char)(v > 2 ? 2 : v);
    }
    /* new pairs: t plus an earlier term */
    for (i = 0; i < a7087_cnt; i++) {
        size_t s = (size_t)(a7087_terms[i] + t);
        if (r2[s] < 2) r2[s]++;
    }
    if (a7087_cnt == a7087_cap) {
        a7087_cap = a7087_cap ? 2 * a7087_cap : 64;
        a7087_terms = (long long *)realloc(a7087_terms, a7087_cap * sizeof(long long));
    }
    a7087_terms[a7087_cnt++] = t;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a7087_cnt == 0) {
        a7087_add(1);
        a7087_add(2);
        a7087_add(4);
    }
    while ((idx_t)a7087_cnt < n) {
        long long t = a7087_terms[a7087_cnt - 1];
        long long m, found = -1;
        for (m = t + 1; m <= 3 * t; m++) {
            if (a7087_r3[(size_t)m] == 1) { found = m; break; }
        }
        if (found < 0) break;
        a7087_add(found);
    }
    if ((idx_t)a7087_cnt < n) return 0;
    return (seq_t)(unsigned long long)a7087_terms[n - 1];
}
