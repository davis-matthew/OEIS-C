/*
 * A003942  Order of simple Chevalley group B_6(q), q = prime power.
 * https://oeis.org/A003942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(A000961(n+1), 6), where b(q,6) = q^36 *
 *            Product_{i=1..6} (q^(2i) - 1) / gcd(2, q-1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a000248_pow.h"

static uint64_t *pp_list = NULL;   /* pp_list[0] = 1, pp_list[i] = (i+1)-th prime power */
static idx_t pp_count = 0;

static void build_prime_powers(idx_t need) {
    uint64_t limit = 1024;
    while (pp_count < need) {
        uint32_t *spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
        uint64_t i, j;
        for (i = 2; i <= limit; i++) {
            if (spf[i] == 0) {
                for (j = i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        free(pp_list);
        pp_list = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
        pp_count = 0;
        pp_list[pp_count++] = 1;
        for (i = 2; i <= limit; i++) {
            uint64_t p = spf[i], t = i;
            while (t % p == 0) t /= p;
            if (t == 1) pp_list[pp_count++] = i;
        }
        free(spf);
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    uint64_t qq;
    seq_t q, result;
    idx_t i;

    if (pp_count < n + 1) build_prime_powers(n + 1);
    qq = pp_list[n];               /* A000961(n+1) */
    q = (seq_t)qq;

    result = a000248_pow(q, 36);
    for (i = 1; i <= 6; i++) {
        result *= (a000248_pow(q, 2 * i) - 1);
    }
    if (qq % 2 == 1) result /= 2;  /* gcd(2, q-1) = 2 for odd q */
    return result;
}
