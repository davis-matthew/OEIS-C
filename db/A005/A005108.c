/*
 * A005108  Class 4+ primes (for definition see A005105).
 * https://oeis.org/A005108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p, classified by Erdos-Selfridge: class(p) =
 *            1 + max class(q) over primes q | p+1, with q=2,3 counted as
 *            class 0; a(n) = n-th prime with class(p) = 4
 * Range:     u8 n <= 1, u16 n <= 2029, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *found = NULL;
static idx_t nfound = 0;
static uint64_t cur_limit = 0;

static void build(uint64_t limit) {
    uint32_t *spf = (uint32_t *)malloc((size_t)(limit + 2) * sizeof(uint32_t));
    unsigned char *cls = (unsigned char *)malloc((size_t)(limit + 2));
    uint64_t i, j, cap = 1024, cnt = 0;
    uint64_t *list = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    memset(spf, 0, (size_t)(limit + 2) * sizeof(uint32_t));
    memset(cls, 0, (size_t)(limit + 2));
    for (i = 2; i <= limit + 1; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit + 1)
                for (j = i * i; j <= limit + 1; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 2; i <= limit; i++) {
        if (spf[i] != i) continue;
        if (i == 2 || i == 3) { cls[i] = 0; continue; }
        {
            uint64_t m = i + 1;
            unsigned char mx = 0;
            while (m > 1) {
                uint64_t q = spf[m];
                if (cls[q] > mx) mx = cls[q];
                while (m % q == 0) m /= q;
            }
            cls[i] = (unsigned char)(mx + 1);
        }
        if (cls[i] == 4) {
            if (cnt == cap) {
                cap *= 2;
                list = (uint64_t *)realloc(list, (size_t)cap * sizeof(uint64_t));
            }
            list[cnt++] = i;
        }
    }
    free(spf);
    free(cls);
    free(found);
    found = list;
    nfound = cnt;
    cur_limit = limit;
}

seq_t a(idx_t n) {
    if (cur_limit == 0) build(1 << 16);
    while (nfound < n) build(cur_limit * 2);
    return (seq_t)found[n - 1];
}
