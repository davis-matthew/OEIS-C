/*
 * A194822  a(n) = 3+floor( Sum_{k=1..n} <((-1)^k)*k*(1+sqrt(5))/2> ), where <
 *          > = fractional part.
 * https://oeis.org/A194822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 57, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A194822
 *  a(n) = 3 + floor( Σ_{k=1..n} (-1)^k * < k*(1+√5)/2 > ),
 *  where <x> denotes the fractional part of x.
 *
 *  Implementation uses only integer arithmetic.
 *  floor_phi_of_seq(m) returns ⌊φ*m⌋ for any non‑negative integer m,
 *  where φ = (1+√5)/2.
 *  The function relies on the utility floor_sqrt which returns
 *  ⌊√x⌋ for arbitrary‐precision integers seq_t.
 */

#include "../utils/floor_sqrt.h"

/* Compute floor( φ * m ) for m >= 0 */
static seq_t floor_phi_of_seq(seq_t m)
{
    if (m == (seq_t)0) {
        return (seq_t)0;
    }
    seq_t m_sq   = m * m;                 /* m² */
    seq_t five_m_sq = m_sq * (seq_t)5;    /* 5*m² */
    seq_t s      = floor_sqrt(five_m_sq); /* floor(sqrt(5*m²)) = floor(m√5) */
    seq_t sum    = m + s;                 /* m + floor(m√5) */

    /* floor(φ*m) = floor( (m + floor(m√5)) / 2 ) or (sum-1)/2 if sum odd */
    if ((sum % (seq_t)2) == (seq_t)0) {
        return sum / (seq_t)2;
    } else {
        return (sum - (seq_t)1) / (seq_t)2;
    }
}

seq_t a(idx_t n)
{
    /* 0 is not part of the official offset; return 3 for safety */
    if (n == (idx_t)0) {
        return (seq_t)3;
    }

    /* T = Σ_{k=1..n} (-1)^k * k  (closed form) */
    seq_t T;
    if ((n & (idx_t)1) == 0) {
        T = (seq_t)(n / (idx_t)2);
    } else {
        T = -(seq_t)((n + (idx_t)1) / (idx_t)2);
    }

    /* floor(φ*T) */
    seq_t floor_phi_T;
    if (T >= (seq_t)0) {
        floor_phi_T = floor_phi_of_seq(T);
    } else {
        seq_t p = -T;
        seq_t fp = floor_phi_of_seq(p);
        floor_phi_T = -(fp + (seq_t)1);  /* floor(-x) = -(ceil(x)) for non‑integer x */
    }

    /* Σ_{k=1..n} (-1)^k * floor(k φ) */
    seq_t altSum = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t floor_k_phi = floor_phi_of_seq((seq_t)k);
        if ((k & (idx_t)1) == 0) {    /* even k : + */
            altSum += floor_k_phi;
        } else {                      /* odd  k : - */
            altSum -= floor_k_phi;
        }
    }

    /* floor of the sum inside the floor() in the definition */
    seq_t floor_S = floor_phi_T - altSum;

    /* final value a(n) */
    return (seq_t)3 + floor_S;
}
