/*
 * A073646  Least number formed by concatenating the prime factors of n in
 *          base 10.
 * https://oeis.org/A073646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least decimal formed by concatenating the multiset of
 *            prime factors of n such that the resulting integer is
 *            lexicographically minimal. For n = 1 the value is 1
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, factor_u64, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
/* utils needed for factorization, string length, pow10 and multiplication */
#include "../utils/factor_u64.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

/* comparator for two primes according to the concatenation rule
   returns negative if a < b, positive if a > b, zero if equal */
static int cmp_prime(const void *va, const void *vb) {
    uint64_t a = *(const uint64_t *)va;
    uint64_t b = *(const uint64_t *)vb;

    seq_t left  = seq_mul((seq_t)a,
                          pow10((idx_t)digit_len((seq_t)b)));
    left += (seq_t)b;

    seq_t right = seq_mul((seq_t)b,
                          pow10((idx_t)digit_len((seq_t)a)));
    right += (seq_t)a;

    if (left < right) return -1;
    if (left > right) return  1;
    return 0;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;          /* defined in OEIS as a(1) = 1 */

    /* factorization: primes[i]^exp[i], count = cnt */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* build the list of prime factors, accounting for multiplicities */
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i)
        total += exps[i];

    uint64_t *fac = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!fac) return 0;   /* fail safe – unlikely for n≤10000 */

    size_t pos = 0;
    for (size_t i = 0; i < cnt; ++i)
        for (uint64_t j = 0; j < exps[i]; ++j)
            fac[pos++] = primes[i];

    /* sort to obtain the lexicographically smallest concatenation */
    qsort(fac, total, sizeof(uint64_t), cmp_prime);

    /* build the resulting integer by concatenation */
    seq_t result = 0;
    for (size_t i = 0; i < total; ++i) {
        seq_t p   = fac[i];
        int len   = digit_len(p);
        result = seq_mul(result, pow10((idx_t)len));
        result += p;
    }

    free(fac);
    return result;
}
