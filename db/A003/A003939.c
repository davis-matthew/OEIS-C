/*
 * A003939  Order of simple Chevalley group B_3(q), q = prime power.
 * https://oeis.org/A003939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^9*(q^2-1)*(q^4-1)*(q^6-1)/gcd(2,q-1), where q =
 *            A000961(n+1) is the n-th prime power > 1
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a000272_ipow.h"

static uint64_t *pp_list = NULL;
static idx_t pp_count = 0;
static uint64_t pp_limit = 0;

/* list prime powers > 1 in increasing order up to limit */
static void build_prime_powers(uint64_t limit) {
    bool *comp = (bool *)calloc(limit + 1, sizeof(bool));
    bool *ispp = (bool *)calloc(limit + 1, sizeof(bool));
    uint64_t i, j, cnt = 0;
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = true;
    for (i = 2; i <= limit; i++) {
        if (!comp[i]) {
            uint64_t pw = i;
            for (;;) {
                ispp[pw] = true;
                if (pw > limit / i) break;
                pw *= i;
            }
        }
    }
    for (i = 2; i <= limit; i++) if (ispp[i]) cnt++;
    free(pp_list);
    pp_list = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    pp_count = 0;
    for (i = 2; i <= limit; i++) if (ispp[i]) pp_list[pp_count++] = i;
    pp_limit = limit;
    free(comp);
    free(ispp);
}

seq_t a(idx_t n) {
    if (pp_limit == 0) build_prime_powers(200000);
    while (n > pp_count) build_prime_powers(pp_limit * 2);

    uint64_t qq = pp_list[n - 1];
    seq_t q = (seq_t)qq;
    seq_t q2 = q * q;
    seq_t f1 = q2 - 1;
    if (qq % 2 == 1) f1 /= 2;
    seq_t q4 = q2 * q2;
    seq_t q6 = q4 * q2;
    seq_t q9 = a000272_ipow(q, 9);
    return q9 * f1 * (q4 - 1) * (q6 - 1);
}
