/*
 * A003944  Order of simple Chevalley group B_8(q), q = prime power.
 * https://oeis.org/A003944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(A000961(n+1), 8) with b(q,8) = q^64 * Prod_{i=1..8}
 *            (q^(2i) - 1) / gcd(2, q-1), i.e. q runs over the prime powers 2,
 *            3, 4, 5, 7, 8, 9,
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *pp_list = NULL;
static size_t pp_count = 0;
static uint64_t pp_limit = 0;

static void build_prime_powers(uint64_t limit) {
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    unsigned char *isppw = (unsigned char *)calloc((size_t)limit + 1, 1);
    uint64_t i, j;
    size_t cnt = 0;
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
            {
                uint64_t pk = i;
                while (pk <= limit) {
                    isppw[pk] = 1;
                    if (pk > limit / i) break;
                    pk *= i;
                }
            }
        }
    }
    for (i = 2; i <= limit; i++) if (isppw[i]) cnt++;
    free(pp_list);
    pp_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    pp_count = 0;
    for (i = 2; i <= limit; i++) if (isppw[i]) pp_list[pp_count++] = i;
    pp_limit = limit;
    free(comp);
    free(isppw);
}

/* k-th prime power > 1, k >= 1 */
static uint64_t kth_prime_power(idx_t k) {
    uint64_t lim = pp_limit ? pp_limit : 1024;
    while (pp_count < (size_t)k) {
        if (pp_limit) lim = pp_limit * 2;
        build_prime_powers(lim);
    }
    return pp_list[k - 1];
}

static seq_t spow(seq_t b, unsigned e) {
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t q = kth_prime_power(n);
    seq_t Q = (seq_t)q;
    seq_t q2 = Q * Q;
    seq_t result = spow(Q, 64);
    seq_t qp = 1;
    int i;
    for (i = 1; i <= 8; i++) {
        seq_t f;
        qp *= q2;
        f = qp - 1;
        if (i == 1 && (q & 1)) f /= 2;
        result *= f;
    }
    return result;
}
