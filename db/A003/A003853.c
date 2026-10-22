/*
 * A003853  Order of simple Chevalley group D_7(q), q = prime power.
 * https://oeis.org/A003853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(A000961(n+1),7), where d(q,7) =
 *            q^42*(q^7-1)*Prod_{i=1..6}(q^(2i)-1)/gcd(4,q^7-1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 1, big any n
 * Helpers:   pow_q
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_q.h"

static uint64_t *pp_list = NULL;   /* prime powers > 1, ascending */
static size_t pp_count = 0;

static void build_pp(size_t need) {
    uint64_t limit = 1024;
    while (pp_count < need) {
        uint32_t *spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        uint64_t i, j;
        size_t cnt = 0;
        uint64_t *lst;
        for (i = 2; i <= limit; i++) {
            if (spf[i] == 0) {
                for (j = i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        lst = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
        for (i = 2; i <= limit; i++) {
            uint64_t p = spf[i], t = i;
            while (t % p == 0) t /= p;
            if (t == 1) lst[cnt++] = i;
        }
        free(spf);
        if (cnt >= need) {
            free(pp_list);
            pp_list = lst;
            pp_count = cnt;
        } else {
            free(lst);
            limit *= 2;
        }
    }
}

seq_t a(idx_t n) {
    uint64_t q;
    seq_t qs, r, f;
    unsigned g;
    int i;
    build_pp((size_t)n);
    q = pp_list[n - 1];
    qs = (seq_t)q;
    if (q % 2 == 0) g = 1;
    else if (q % 4 == 1) g = 4;
    else g = 2;
    r = pow_q(qs, 42);
    f = pow_q(qs, 7) - 1;
    f /= g;
    r *= f;
    for (i = 1; i <= 6; i++)
        r *= pow_q(qs, 2 * i) - 1;
    return r;
}
