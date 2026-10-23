/*
 * A047924  a(n) = B_{A_n+1}+1, where A_n = floor(n*phi) = A000201(n), B_n =
 *          floor(n*phi^2) = A001950(n) and phi is the golden ratio.
 * https://oeis.org/A047924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = B_{A_n+1} + 1, where A_n = floor(n*phi), B_n =
 *            floor(n*phi^2), phi = (1+sqrt(5))/2
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Scaled integer representation of sqrt(5):
   sqrt(5) ≈ 2.23606797749979
   *10^14 = 223606797749979  (fits in 64 bits). */
static const unsigned long long SQRT5_NUM = 223606797749979ULL;
static const unsigned long long SQRT5_DEN = 100000000000000ULL;

/* Compute floor(k * sqrt(5)) using integer arithmetic. */
static inline seq_t floor_k_sqrt5(seq_t k)
{
    return (seq_t)(((unsigned __int128)k * SQRT5_NUM) / SQRT5_DEN);
}

seq_t a(idx_t n)
{
    seq_t n_t   = (seq_t)n;
    seq_t m_n   = floor_k_sqrt5(n_t);
    seq_t A_n   = n_t + (m_n - n_t) / 2;        /* A_n = floor(n*phi) */

    seq_t an1   = A_n + 1;
    seq_t m_an1 = floor_k_sqrt5(an1);
    seq_t A_an1 = an1 + (m_an1 - an1) / 2;      /* A_{A_n+1} */

    return A_an1 + A_n + 2;                    /* a(n) = B_{A_n+1} + 1 */
}
