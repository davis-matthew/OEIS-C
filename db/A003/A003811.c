/*
 * A003811  Order of simple Chevalley group A_6(q), q = prime power.
 * https://oeis.org/A003811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            q^21*(q^2-1)*(q^3-1)*(q^4-1)*(q^5-1)*(q^6-1)*(q^7-1)/gcd(7,q-1),
 *            q = A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 4, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *pp_list = NULL;
static idx_t pp_count = 0;
static uint64_t pp_limit = 0;

static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static void build_prime_powers(uint64_t limit) {
    bool *comp = (bool *)calloc(limit + 1, sizeof(bool));
    /* mark[m] = 1 if m is a prime power */
    bool *ispp = (bool *)calloc(limit + 1, sizeof(bool));
    uint64_t i, j;
    idx_t cnt = 0;
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            for (j = i * i; j <= limit; j += i) comp[j] = true;
            {
                uint64_t pw = i;
                while (pw <= limit) {
                    ispp[pw] = true;
                    if (pw > limit / i) break;
                    pw *= i;
                }
            }
        }
    }
    for (i = 2; i <= limit; i++) if (ispp[i]) cnt++;
    free(pp_list);
    pp_list = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(cnt + 1));
    pp_list[0] = 1;
    pp_count = 1;
    for (i = 2; i <= limit; i++) if (ispp[i]) pp_list[pp_count++] = i;
    pp_limit = limit;
    free(comp);
    free(ispp);
}

/* A000961(k), 1-indexed: 1, 2, 3, 4, 5, 7, ... */
static uint64_t nth_prime_power(idx_t k) {
    while (pp_count < k) {
        uint64_t lim = pp_limit ? pp_limit * 2 : 200000;
        build_prime_powers(lim);
    }
    return pp_list[k - 1];
}

seq_t a(idx_t n) {
    uint64_t q = nth_prime_power(n + 1);
    seq_t qs = (seq_t)q;
    seq_t res = 1;
    seq_t qi;
    int i;
    for (i = 0; i < 21; i++) res *= qs;
    qi = qs;
    for (i = 2; i <= 7; i++) {
        qi *= qs;
        res *= (qi - 1);
    }
    res /= (seq_t)gcd_u64(7, q - 1);
    return res;
}
