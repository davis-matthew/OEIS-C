/*
 * A154364  Number of ways to express n as the sum of an odd prime, a positive
 *          Pell number and a companion Pell number.
 * https://oeis.org/A154364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 578, u16 n <= 9397, u32 n <= 9397, u64 n <= 9397,
 *            u128 n <= 9397, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  A154364 – Number of ways to express n as the sum of an odd prime,
 *  a positive Pell number and a companion Pell (Lucas‑Pell) number.
 *  The algorithm generates the Pell (P) and companion Pell (C) values
 *  up to n, then counts solutions p + P[i] + C[j] = n with p odd
 *  prime. Only values that fit in seq_t are generated, so the
 *  implementation works for all n ≥ 1.
 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Max number of Pell / companion Pell values needed for n up to 2^64‑1.
 * Pell numbers grow roughly as (1+√2)^k; 64 entries exceed 2^128
 * and are more than enough for any test case.  The static arrays are
 * on the stack and are re‑initialised on each call. */
#define MAX_PELL 64

seq_t a(idx_t n) {
    if (n < 1) return 0;

    seq_t N = (seq_t)n;

    /* Generating Pell values P[0] = 0, P[1] = 1, P[k] = 2*P[k‑1] + P[k‑2] */
    seq_t P[MAX_PELL];
    size_t p_len = 0;
    P[0] = 0;
    if (MAX_PELL > 1) {
        P[1] = 1;
        p_len = 2;
    } else {
        p_len = 1;
    }
    while (p_len < MAX_PELL) {
        seq_t next = 2 * P[p_len - 1] + P[p_len - 2];
        if (next >= N) break;          /* we only need values < n */
        P[p_len++] = next;
    }

    /* Generating companion Pell (Lucas‑Pell) values C[0] = 2, C[1] = 6,
     *  C[k] = 2*C[k‑1] + C[k‑2]   */
    seq_t C[MAX_PELL];
    size_t c_len = 0;
    C[0] = 2;
    if (MAX_PELL > 1) {
        C[1] = 6;
        c_len = 2;
    } else {
        c_len = 1;
    }
    while (c_len < MAX_PELL) {
        seq_t next = 2 * C[c_len - 1] + C[c_len - 2];
        if (next >= N) break;          /* only values < n are needed */
        C[c_len++] = next;
    }

    seq_t count = 0;

    /* Count all triples p + P[i] + C[j] = n with p odd prime */
    for (size_t i = 1; i < p_len; ++i) {           /* P[0] = 0 is not positive */
        seq_t p_val = P[i];
        if (p_val >= N) break;                     /* remaining values only larger */
        for (size_t j = 0; j < c_len; ++j) {
            seq_t c_val = C[j];
            seq_t sum = p_val + c_val;
            if (sum >= N) break;                   /* C is increasing */
            seq_t rest = N - sum;
            if (rest <= 2 || (rest & 1ULL) == 0)   /* require odd prime > 2 */
                continue;
            if (is_prime_big(rest))
                ++count;
        }
    }

    return count;
}
