/*
 * A003803  Order of universal Chevalley group A_5 (q), q = prime power.
 * https://oeis.org/A003803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^15 * Product_{i=2..6} (q^i - 1), where q = A000961(n+1)
 *            (order of SL_6(q))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 7, big any n
 * Helpers:   spow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/spow.h"

static uint64_t *pp_list = NULL;
static size_t pp_cnt = 0;
static size_t pp_limit = 0;

static void build_pp(size_t limit) {
    uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    size_t i, j, cnt = 0;
    uint64_t *list;
    for (i = 2; i <= limit; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    list = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    list[cnt++] = 1;
    for (i = 2; i <= limit; i++) {
        size_t t = i;
        uint32_t p = spf[i];
        while (t % p == 0) t /= p;
        if (t == 1) list[cnt++] = (uint64_t)i;
    }
    free(spf);
    free(pp_list);
    pp_list = list;
    pp_cnt = cnt;
    pp_limit = limit;
}

/* m-th prime power (A000961), 1-based: A000961(1) = 1 */
static uint64_t prime_power(uint64_t m) {
    size_t lim = pp_limit ? pp_limit : 1024;
    if (pp_limit == 0) build_pp(lim);
    while (pp_cnt < m) {
        lim *= 2;
        build_pp(lim);
    }
    return pp_list[m - 1];
}

seq_t a(idx_t n) {
    uint64_t qn = prime_power((uint64_t)n + 1);
    seq_t q = (seq_t)qn;
    seq_t result = spow(q, 15);
    seq_t qi = q;
    unsigned i;
    for (i = 2; i <= 6; i++) {
        qi = qi * q;
        result = result * (qi - 1);
    }
    return result;
}
