/*
 * A190496  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),3,2) and
 *          []=floor.
 * https://oeis.org/A190496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3n+2)*sqrt(2)) - 3*floor(n*sqrt(2)) - 2
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* b = 3, c = 2 */
    seq_t bn = (seq_t)3 * n_seq;
    seq_t k  = bn + (seq_t)2;          /* k = 3n + 2 */

    /* Compute floor(n*sqrt(2))  via floor_sqrt(2*n^2) */
    seq_t n_sq   = n_seq * n_seq;
    seq_t floor_nr = floor_sqrt((seq_t)2 * n_sq);

    /* Compute floor(k*sqrt(2))  via floor_sqrt(2*k^2) */
    seq_t k_sq   = k * k;
    seq_t floor_kr = floor_sqrt((seq_t)2 * k_sq);

    /* a(n) = floor_kr - 3*floor_nr - 2 */
    seq_t temp  = floor_kr - (seq_t)3 * floor_nr;
    seq_t result = temp - (seq_t)2;

    return result;
}
