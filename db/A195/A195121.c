/*
 * A195121  a(n) = 2*n - floor(n/r), where r = (1 + sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A195121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(n*phi) (phi = (1+√5)/2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Convergent P/Q < phi with error < 1/Q^2
   P = F60, Q = F59  */
#define PHI_P 1548008755920ULL
#define PHI_Q 956722026041ULL

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to keep all arithmetic in seq_t */
    seq_t n_seq = (seq_t)n;

    /* Compute floor(n * phi) ≈ floor(n * P / Q) */
    seq_t prod        = n_seq * (seq_t)PHI_P;
    seq_t floor_nphi  = prod / (seq_t)PHI_Q;

    /* a(n) = 3*n - floor(n*phi) */
    seq_t result = (seq_t)3 * n_seq - floor_nphi;

    return result;
}
