/*
 * A099108  Numbers n such that A005382(n) + A005384(n) - 1 and A005382(n) +
 *          A005384(n) + 1 are twin primes.
 * https://oeis.org/A099108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index k>=3 such that A005382(k)+A005384(k)-1 and
 *            A005382(k)+A005384(k)+1 are both prime (search; k=1,2 excluded)
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t *tw_L1 = NULL, *tw_L2 = NULL, *tw_R = NULL;
static size_t tw_n1 = 0, tw_n2 = 0, tw_nr = 0;
static size_t tw_c1 = 0, tw_c2 = 0, tw_cr = 0;
static uint64_t tw_next1 = 2, tw_next2 = 2, tw_nidx = 3;

static void tw_push(uint64_t **arr, size_t *len, size_t *cap, uint64_t v) {
    if (*len == *cap) {
        size_t nc = *cap ? *cap * 2 : 1024;
        *arr = (uint64_t *)realloc(*arr, nc * sizeof(uint64_t));
        *cap = nc;
    }
    (*arr)[(*len)++] = v;
}

static uint64_t tw_step(uint64_t p) {
    return p == 2 ? 3 : p + 2;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (tw_nr < (size_t)n) {
        uint64_t idx = tw_nidx;
        while (tw_n1 < idx) {
            uint64_t p = tw_next1;
            while (!(is_prime_u64(p) && is_prime_u64(2 * p - 1))) p = tw_step(p);
            tw_push(&tw_L1, &tw_n1, &tw_c1, p);
            tw_next1 = tw_step(p);
        }
        while (tw_n2 < idx) {
            uint64_t p = tw_next2;
            while (!(is_prime_u64(p) && is_prime_u64(2 * p + 1))) p = tw_step(p);
            tw_push(&tw_L2, &tw_n2, &tw_c2, p);
            tw_next2 = tw_step(p);
        }
        uint64_t s = tw_L1[idx - 1] + tw_L2[idx - 1];
        if (is_prime_u64(s - 1) && is_prime_u64(s + 1)) {
            tw_push(&tw_R, &tw_nr, &tw_cr, idx);
        }
        tw_nidx++;
    }
    return (seq_t)tw_R[(size_t)(n - 1)];
}
