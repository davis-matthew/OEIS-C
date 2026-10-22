/*
 * A003804  Order of universal Chevalley group A_6(q), q = prime power.
 * https://oeis.org/A003804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^21 * Product_{i=2..7} (q^i - 1), where q = A000961(n+1)
 *            is the (n+1)-th prime power (A000961(1) = 1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 4, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *pp_list = NULL;
static uint64_t pp_count = 0;

static void build_prime_powers(uint64_t need) {
    uint64_t limit = 64;
    while (pp_count < need) {
        limit *= 2;
        unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
        bool *ispp = (bool *)calloc(limit + 1, sizeof(bool));
        uint64_t i, j, cnt = 0;
        for (i = 2; i <= limit; i++) {
            if (!comp[i]) {
                uint64_t pk;
                for (j = i * i; j <= limit; j += i) comp[j] = 1;
                for (pk = i; pk <= limit; ) {
                    ispp[pk] = true;
                    if (pk > limit / i) break;
                    pk *= i;
                }
            }
        }
        for (i = 2; i <= limit; i++) if (ispp[i]) cnt++;
        free(pp_list);
        pp_list = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
        pp_list[0] = 1;
        pp_count = 1;
        for (i = 2; i <= limit; i++) if (ispp[i]) pp_list[pp_count++] = i;
        free(comp);
        free(ispp);
    }
}

seq_t a(idx_t n) {
    uint64_t q;
    seq_t Q, pw, result;
    int i;
    build_prime_powers((uint64_t)n + 1);
    q = pp_list[n];  /* A000961(n+1), 0-based index n */
    Q = (seq_t)q;
    result = 1;
    pw = 1;
    for (i = 1; i <= 7; i++) {
        pw *= Q;
        if (i >= 2) result *= (pw - 1);
    }
    for (i = 0; i < 21; i++) result *= Q;
    return result;
}
