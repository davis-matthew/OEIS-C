/*
 * A003806  Order of universal Chevalley group A_8 (q), q = prime power.
 * https://oeis.org/A003806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^36 * Product_{i=2..9} (q^i - 1), where q = A000961(n+1)
 *            (the (n+1)-th prime power, counting 1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *pp_list = NULL;
static idx_t pp_count = 0;
static uint64_t pp_limit = 0;

static void build_prime_powers(uint64_t limit) {
    uint32_t *spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    uint64_t i, j;
    idx_t cnt = 0;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    free(pp_list);
    pp_list = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    pp_list[cnt++] = 1;
    for (i = 2; i <= limit; i++) {
        uint64_t p = spf[i], t = i;
        while (t % p == 0) t /= p;
        if (t == 1) pp_list[cnt++] = i;
    }
    free(spf);
    pp_count = cnt;
    pp_limit = limit;
}

static uint64_t nth_prime_power(idx_t k) {
    uint64_t limit = pp_limit ? pp_limit : 1024;
    while (pp_count < k) {
        if (pp_limit) limit *= 2;
        build_prime_powers(limit);
    }
    return pp_list[k - 1];
}

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    idx_t i;
    for (i = 0; i < exp; i++) result *= base;
    return result;
}

seq_t a(idx_t n) {
    uint64_t qu = nth_prime_power(n + 1);
    seq_t q = (seq_t)qu;
    seq_t result = pow_seq(q, 36);
    seq_t qi = q;
    idx_t i;
    for (i = 2; i <= 9; i++) {
        qi *= q;
        result *= (qi - 1);
    }
    return result;
}
