/*
 * A079167  Weighted roundness of n. If n = p_1^e_1...p_k^e_k, then a(n) = e_1
 *          + 2*e_2 + ... + k*e_k. Note that p_i < p_j, i < j is assumed.
 * https://oeis.org/A079167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weighted roundness (A079167) computed by prime factorisation.
   For n ≥ 1, n = ∏_{i=1..k} p_i^{e_i} with p_i increasing,
   a(n) = ∑_{i=1..k} i * e_i.  For n = 1 a(1) = 0. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Treat n as an unsigned 64‑bit integer for the factorisation loop. */
    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;
    unsigned int weight = 1;          /* 1‑based index of the current prime factor */
    uint64_t p = 2;                   /* current trial divisor */

    while (nn > 1 && p * p <= nn) {
        if (nn % p == 0) {
            unsigned int exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            sum += (seq_t)weight * exp;
            weight++;
        }
        p++;
    }

    if (nn > 1) {                /* remaining prime factor with exponent 1 */
        sum += (seq_t)weight * 1;
    }

    return sum;
}
