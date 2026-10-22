/*
 * A003813  Order of simple Chevalley group A_8(q), q = prime power.
 * https://oeis.org/A003813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^36 * Product_{i=2..9} (q^i - 1) / gcd(9, q-1), where q
 *            = A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   pp_gcd, pp_is_prime_power, pp_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pp_gcd.h"
#include "../utils/pp_is_prime_power.h"
#include "../utils/pp_pow.h"

static uint64_t *pp_cache = NULL;
static idx_t pp_len = 0, pp_cap = 0;
static uint64_t pp_next = 2;

/* k-th term of A000961 (1-indexed; A000961(1) = 1) */
static uint64_t pp_get(idx_t k) {
    if (k <= 1) return 1;
    while (pp_len < k - 1) {
        if (pp_is_prime_power(pp_next)) {
            if (pp_len == pp_cap) {
                pp_cap = pp_cap ? pp_cap * 2 : 1024;
                pp_cache = (uint64_t *)realloc(pp_cache, pp_cap * sizeof(uint64_t));
            }
            pp_cache[pp_len++] = pp_next;
        }
        pp_next++;
    }
    return pp_cache[k - 2];
}

seq_t a(idx_t n) {
    uint64_t q = pp_get(n + 1);
    seq_t qq = (seq_t)q;
    uint64_t g = pp_gcd(9, q - 1);

    /* gcd(9, q-1) divides q-1, hence q^2-1: divide there exactly */
    seq_t result = (qq * qq - 1) / (seq_t)g;
    for (int i = 3; i <= 9; i++)
        result *= pp_pow(qq, i) - 1;
    result *= pp_pow(qq, 36);
    return result;
}
