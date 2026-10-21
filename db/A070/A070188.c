/*
 * A070188  Primes p such that x^12 = 2 has a solution mod p, but x^(12^2) = 2
 *          has no solution mod p.
 * https://oeis.org/A070188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that x^12 = 2 (mod p) has a solution but x^144 = 2
 *            (mod p) has no solution
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   ensure_capacity, factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_capacity.h"

/* Compute gcd of two unsigned 64‑bit integers */
static inline uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* State for memoisation */
typedef struct {
    seq_t *data;
    size_t sz;
    size_t cap;
    idx_t next_start;               /* next prime to test                */
} seq_state_t;

static seq_state_t state = {NULL, 0, 0, 3};

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for 0, but safe */

    /* Produce terms until we have at least n */
    while (state.sz < (size_t)n) {
        /* Get next prime */
        uint64_t p = next_prime(state.next_start);
        state.next_start = p + 1;
        if (p < 3) continue;                 /* skip 2 */

        /* Compute d = p-1 */
        seq_t d = (seq_t)(p - 1);

        /* Compute factorization of d */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t cnt = 0;
        factor_u64((uint64_t)d, pr, ex, &cnt);

        /* Compute multiplicative order of 2 modulo p */
        seq_t ord = d;                       /* start with d */
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t q = pr[i];
            while (ord % q == 0) {
                seq_t test_exp = ord / q;
                if (mod_pow((seq_t)2, (idx_t)test_exp, (seq_t)p) == 1)
                    ord /= q;
                else
                    break;
            }
        }

        /* Compute t1 = gcd(12, d) and t2 = gcd(144, d) */
        uint64_t t1 = ugcd(12ULL, (uint64_t)d);
        uint64_t t2 = ugcd(144ULL, (uint64_t)d);
        seq_t e1 = d / (seq_t)t1;
        seq_t e2 = d / (seq_t)t2;

        /* Check conditions: e1 % ord == 0 and e2 % ord != 0 */
        if ((e1 % ord == 0) && (e2 % ord != 0)) {
            /* Store the prime */
            ensure_capacity(state.sz + 1, &state.cap, (void **)&state.data,
                            sizeof(seq_t));
            state.data[state.sz++] = (seq_t)p;
        }
    }
    return state.data[n - 1];
}
