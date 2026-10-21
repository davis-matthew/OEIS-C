/*
 * A079631  Number of k less than or equal to n such that floor(k^(1/3))
 *          divides k.
 * https://oeis.org/A079631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / floor(n^(1/3))) + (floor(n^(1/3))^2 +
 *            5*floor(n^(1/3)) - 6)/2
 * Range:     u8 n <= 277, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cube_root_floor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not defined for n=0, but guard anyway */

    idx_t m = cube_root_floor(n);     /* floor(cuberoot(n))   */
    seq_t n_seq = (seq_t)n;
    seq_t m_seq = (seq_t)m;
    seq_t term1 = n_seq / m_seq;                      /* floor(n / m)      */
    seq_t m_sq  = m_seq * m_seq;                      /* m^2               */
    seq_t term2 = (m_sq + 5 * m_seq - 6) / 2;         /* (m^2 + 5m - 6)/2 */
    return term1 + term2;
}
