/*
 * A003931  Order of universal Chevalley group B_2(q), q = prime power.
 * https://oeis.org/A003931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^4*(q^2-1)*(q^4-1) where q = A000961(n+1), the n-th
 *            prime power > 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 33, u128 n <= 962,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *pp = NULL;
static idx_t pp_len = 0;
static idx_t pp_cap = 0;
static uint64_t pp_next = 2;

static bool is_prime_power_u64(uint64_t k) {
    uint64_t p;
    if (k < 2) return false;
    for (p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            while (k % p == 0) k /= p;
            return k == 1;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t qv;
    seq_t q, q2, q4;
    while (pp_len < n) {
        if (pp_len >= pp_cap) {
            idx_t nc = pp_cap ? pp_cap * 2 : 1024;
            pp = (uint64_t *)realloc(pp, (size_t)nc * sizeof(uint64_t));
            pp_cap = nc;
        }
        while (pp_len < pp_cap && pp_len < n) {
            if (is_prime_power_u64(pp_next)) pp[pp_len++] = pp_next;
            pp_next++;
        }
    }
    qv = pp[n - 1];
    q = (seq_t)qv;
    q2 = q * q;
    q4 = q2 * q2;
    return q4 * (q2 - 1) * (q4 - 1);
}
