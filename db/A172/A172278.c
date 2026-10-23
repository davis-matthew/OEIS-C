/*
 * A172278  a(n) = floor(n*(sqrt(13)-sqrt(2))).
 * https://oeis.org/A172278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(13)-sqrt(2)))
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t n_sq = (seq_t)n * (seq_t)n;           /* n² */
    seq_t A = floor_sqrt((seq_t)13 * n_sq);    /* floor(n*sqrt(13)) */
    seq_t B = floor_sqrt((seq_t)2  * n_sq);    /* floor(n*sqrt(2))  */
    seq_t D = A - B;                           /* difference of floors */

    /* Compute L = 11*n² - D² and compare L² with 8*D²*n² */
    unsigned long long n_sq_ll = (unsigned long long)n_sq;
    unsigned long long D_ll = (unsigned long long)D;
    unsigned long long L = 11ULL * n_sq_ll - D_ll * D_ll;
    unsigned long long Lsq = L * L;
    unsigned long long rhs = 8ULL * D_ll * D_ll * n_sq_ll;

    if (Lsq < rhs) {
        return D - 1;
    }
    return D;
}
