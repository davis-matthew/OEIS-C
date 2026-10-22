/*
 * A005112  Class 4- primes (for definition see A005109).
 * https://oeis.org/A005112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search. Class 1-: p-1 has no prime factor > 3 (2 and 3 are
 *            class 1-). Otherwise class(p) = 1 + max class(q) over primes q >
 *            3 dividing p-1. a(n) = n-th prime of class 4-
 * Range:     u8 n <= 4, u16 n <= 2075, u32 n <= 9233, u64 n <= 9233,
 *            u128 n <= 9233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *terms = NULL;
static size_t nterms = 0;
static uint64_t lim = 0;

static void compute(uint64_t L) {
    unsigned char *f = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t *t = (uint64_t *)malloc(sizeof(uint64_t) * 16);
    size_t cap = 16, cnt = 0;
    uint64_t p, m;
    for (p = 2; p <= L; p++) {
        unsigned c;
        if (f[p] & 0x80) continue;
        if (p <= 3) {
            c = 0; /* contributes nothing to the class of later primes */
        } else {
            c = (unsigned)(f[p - 1] & 0x7f) + 1;
            if (c == 4) {
                if (cnt == cap) {
                    cap *= 2;
                    t = (uint64_t *)realloc(t, sizeof(uint64_t) * cap);
                }
                t[cnt++] = p;
            }
        }
        for (m = p; m <= L; m += p) {
            if (m > p) f[m] |= 0x80;
            if (c > (unsigned)(f[m] & 0x7f))
                f[m] = (unsigned char)((f[m] & 0x80) | c);
        }
    }
    free(f);
    free(terms);
    terms = t;
    nterms = cnt;
    lim = L;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (lim == 0) compute(1 << 14);
    while (nterms < n) compute(lim * 2);
    return (seq_t)terms[n - 1];
}
