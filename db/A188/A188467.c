/*
 * A188467  [4r]-[nr]-[4r-nr], where r=(1+sqrt(5))/2 and [.]=floor.
 * https://oeis.org/A188467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [4φ]-[nφ]-[4φ-nφ] where φ = (1+sqrt(5))/2 * Implements
 *            a(n) = 6 - floor(n*φ) - floor((4-n)*φ). * floor(n*φ) is computed
 *            as floor((n + floor(sqrt(5*n*n)))/2). * For negative k,
 *            floor(k*φ) = -floor(|k|*φ) - 1 since φ is irrational
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 179, u64 n <= 179,
 *            u128 n <= 179, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* floor(n*φ) */
    seq_t nn = (seq_t)n;
    seq_t t = 5 * nn * nn;            /* 5 * n * n  */
    seq_t s = floor_sqrt(t);          /* floor(sqrt(5*n*n)) */
    seq_t floor_n_phi = (nn + s) / 2; /* floor(n*φ) */

    /* floor((4-n)*φ) */
    long long m = (long long)4 - (long long)n; /* 4 - n, may be negative */
    long long floor_m_phi;
    if (m >= 0) {
        seq_t mm = (seq_t)m;
        seq_t tm = 5 * mm * mm;
        seq_t sm = floor_sqrt(tm);
        floor_m_phi = (long long)((mm + sm) / 2);
    } else {
        seq_t k = (seq_t)(-m);      /* |m| */
        seq_t tk = 5 * k * k;
        seq_t sk = floor_sqrt(tk);
        seq_t fp = (k + sk) / 2;    /* floor(|m|*φ) */
        floor_m_phi = -(long long)fp - 1;
    }

    long long result = 6 - (long long)floor_n_phi - floor_m_phi;
    return (seq_t)result;
}
