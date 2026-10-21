/*
 * A214990  Second nearest integer to n*r, where r = (1+ sqrt(5))/2, the
 *          golden ratio.
 * https://oeis.org/A214990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*φ) if (n + floor(n*√5)) is odd, floor(n*φ)+1 if
 *            (n + floor(n*√5)) is even, where φ = (1+√5)/2 and √5 is the
 *            square root of 5
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt5_mul_n, mid_sq_greater_than_A
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is the second nearest integer to n*φ.
   All computations are performed with integer arithmetic only.  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt5_mul_n.h"
#include "../utils/mid_sq_greater_than_A.h"

/* Main sequence function */
seq_t a(idx_t n) {
    /* Compute t = floor(n*sqrt(5)) */
    uint64_t t = floor_sqrt5_mul_n(n);

    /* Compute sum = n + t  (fits in 65 bits) */
    unsigned __int128 sum = (unsigned __int128)n + (unsigned __int128)t;

    /* floor(n*φ) = floor(sum/2) */
    unsigned __int128 floor_phi = sum >> 1;

    /* a(n) = floor_phi + (sum even ? 1 : 0)   */
    seq_t result = (seq_t)floor_phi;
    if ((uint64_t)sum & 1ULL) { /* sum is odd → use floor_phi (floor case) */
        /* no addition */
    } else {
        result = (seq_t)(floor_phi + 1);  /* even → ceil case */
    }
    return result;
}
