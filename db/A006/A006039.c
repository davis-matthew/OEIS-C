/*
 * A006039  Primitive nondeficient numbers.
 * https://oeis.org/A006039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve -- sigma(k) for all k <= L by a prime sieve; k is
 *            primitive nondeficient iff sigma(k) >= 2k and no proper divisor
 *            is nondeficient (multiples of each found term are struck out); L
 *            doubles until n terms are found
 * Range:     u8 n <= 6, u16 n <= 327, u32 n <= 8671, u64 n <= 8671,
 *            u128 n <= 8671, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static uint64_t nterms = 0;
static uint64_t sieved = 0;

static void build(uint64_t L) {
    uint32_t *sig = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    uint64_t i, m, cap = 1024, cnt = 0;
    uint64_t *res = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (i = 0; i <= L; i++) sig[i] = 1;
    for (i = 2; i <= L; i++) {
        if (sig[i] != 1) continue; /* i is prime: untouched by smaller primes */
        for (m = i; m <= L; m += i) {
            uint64_t t = m / i, pe = i, s = 1 + i;
            while (t % i == 0) { t /= i; pe *= i; s += pe; }
            sig[m] = (uint32_t)(sig[m] * s);
        }
    }
    for (i = 2; i <= L; i++) {
        if (sig[i] != 0 && (uint64_t)sig[i] >= 2 * i) {
            if (cnt == cap) {
                cap *= 2;
                res = (uint64_t *)realloc(res, (size_t)cap * sizeof(uint64_t));
            }
            res[cnt++] = i;
            for (m = 2 * i; m <= L; m += i) sig[m] = 0;
        }
    }
    free(sig);
    free(terms);
    terms = res;
    nterms = cnt;
    sieved = L;
}

seq_t a(idx_t n) {
    uint64_t L = sieved ? sieved : 4096;
    while (nterms < (uint64_t)n) {
        if (sieved) L *= 2;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
