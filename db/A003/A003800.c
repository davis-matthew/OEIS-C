/*
 * A003800  Order of universal Chevalley group A_2 (q), q = prime power.
 * https://oeis.org/A003800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^3*(q^2-1)*(q^3-1) with q = A000961(n+1), the n-th prime
 *            power > 1 (order of SL(3,q))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   is_prime_power_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_power_u64.h"

static uint64_t *pp_list = NULL;
static idx_t pp_len = 0;
static idx_t pp_cap = 0;
static uint64_t pp_next = 2;

static uint64_t nth_prime_power(idx_t n) {
    while (pp_len < n) {
        if (pp_len == pp_cap) {
            idx_t ncap = pp_cap ? pp_cap * 2 : 1024;
            pp_list = (uint64_t *)realloc(pp_list, ncap * sizeof(uint64_t));
            pp_cap = ncap;
        }
        while (!is_prime_power_u64(pp_next)) pp_next++;
        pp_list[pp_len++] = pp_next++;
    }
    return pp_list[n - 1];
}

seq_t a(idx_t n) {
    uint64_t qq = nth_prime_power(n);
    seq_t q = (seq_t)qq;
    seq_t q2 = q * q;
    seq_t q3 = q2 * q;
    return q3 * (q2 - 1) * (q3 - 1);
}
