/*
 * A231482  The number of nonlinear normal modes for a fully resonant
 *          Hamiltonian system with n degrees of freedom.
 * https://oeis.org/A231482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) * [2*(5*n-7)/(n-1)] - 9*a(n-2),
 * where a(1)=1, a(2)=6.
 * Recurrence (n-1)*a(n) = 2*(5*n-7)*a(n-1) - 9*(n-1)*a(n-2).
 */
#include <stdint.h>

/* Computes the n-th term of OEIS sequence A231482. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    /*
     * For C++ builds (usually arbitrary‑precision integer type),
     * use normal arithmetic since overflow will be handled by the
     * big‑integer implementation.
     */
#ifdef __cplusplus
    seq_t a_prevprev = 1;   /* a(n-2) */
    seq_t a_prev    = 6;    /* a(n-1) */
    for (idx_t m = 3; m <= n; ++m) {
        seq_t term1 = 2 * (seq_t)(5 * m - 7) * a_prev;
        seq_t term2 = 9 * (seq_t)(m - 1) * a_prevprev;
        seq_t num   = term1 - term2;
        seq_t den   = (seq_t)(m - 1);
        seq_t a_curr = num / den;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }
    return a_prev;
#else
    /* For C builds (fixed width integer types),
     * use 128‑bit intermediate arithmetic to prevent overflow. */
    seq_t a_prevprev = 1;   /* a(n-2) */
    seq_t a_prev    = 6;    /* a(n-1) */
    for (idx_t m = 3; m <= n; ++m) {
        __uint128_t term1 = (__uint128_t)2 *
                            (__uint128_t)(5 * m - 7) *
                            (__uint128_t)a_prev;
        __uint128_t term2 = (__uint128_t)9 *
                            (__uint128_t)(m - 1) *
                            (__uint128_t)a_prevprev;
        __uint128_t num   = term1 - term2;
        seq_t a_curr = (seq_t)(num / (m - 1));
        a_prevprev = a_prev;
        a_prev = a_curr;
    }
    return a_prev;
#endif
}
