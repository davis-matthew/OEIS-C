/*
 * A007708  Prime(n)*...*a(n) is the least product of consecutive primes which
 *          is abundant.
 * https://oeis.org/A007708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least prime p >= prime(n) such that
 *            prime(n)*prime(n+1)*...*p is abundant, i.e.
 *            Product_{q=prime(n)..p} (1+1/q) > 2
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *pr = NULL;
static size_t np = 0, pcap = 0;
static uint64_t sieved_to = 0; /* all primes < sieved_to are in pr */

static void push_prime(uint64_t p) {
    if (np == pcap) {
        pcap = pcap ? pcap * 2 : 1024;
        pr = (uint64_t *)realloc(pr, pcap * sizeof(uint64_t));
    }
    pr[np++] = p;
}

static void init_sieve(void) {
    uint64_t lim = (uint64_t)1 << 20, i, j;
    char *c = (char *)malloc(lim);
    memset(c, 1, lim);
    c[0] = c[1] = 0;
    for (i = 2; i * i < lim; i++)
        if (c[i])
            for (j = i * i; j < lim; j += i) c[j] = 0;
    for (i = 2; i < lim; i++)
        if (c[i]) push_prime(i);
    free(c);
    sieved_to = lim;
}

static void extend(void) {
    uint64_t S = (uint64_t)1 << 20;
    uint64_t lo = sieved_to, hi = lo + S, i;
    size_t k;
    char *c = (char *)malloc(S);
    memset(c, 1, S);
    for (k = 0; k < np; k++) {
        uint64_t p = pr[k], st;
        if (p * p >= hi) break;
        st = (lo + p - 1) / p * p;
        if (st < p * p) st = p * p;
        for (; st < hi; st += p) c[st - lo] = 0;
    }
    for (i = 0; i < S; i++)
        if (c[i]) push_prime(lo + i);
    free(c);
    sieved_to = hi;
}

seq_t a(idx_t n) {
    long double ratio = 1.0L;
    size_t k;
    uint64_t p = 0;
    if (sieved_to == 0) init_sieve();
    k = (size_t)(n - 1);
    for (;;) {
        while (k >= np) extend();
        p = pr[k];
        ratio *= (long double)(p + 1) / (long double)p;
        if (ratio > 2.0L + 1e-15L) break;
        k++;
    }
    return (seq_t)p;
}
