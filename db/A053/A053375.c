/*
 * A053375  Write fundamental unit for real quadratic field of discriminant n
 *          as x + y*omega; sequence gives values of y for n == 3 mod 4.
 * https://oeis.org/A053375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 42, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = y in the fundamental unit
 *               ε = x + y ω  of the real quadratic field  
 *                Q(√d) where d is the nth squarefree odd integer ≡ 3 (mod 4).
 * The sequence enumerates the y‑components for those discriminants.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"
#include <stdint.h>
#include <stdbool.h>

/* List of all primes ≤ 200 (there are 46 of them). */
static const uint64_t primes_small[46] = {
     2,  3,  5,  7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97,101,103,107,109,113,
   127,131,137,139,149,151,157,163,167,173,
   179,181,191,193,197,199
};
static const size_t primes_small_cnt = 46;

/* Test whether a positive integer n is square‑free. */
static inline bool is_squarefree(uint64_t n)
{
    for (size_t i = 0; i < primes_small_cnt; ++i) {
        uint64_t p = primes_small[i];
        if (p * p > n)
            break;
        if (n % p == 0) {
            n /= p;
            if (n % p == 0)
                return false;           /* squared prime factor found */
        }
    }
    return true;
}

/* 
 * Compute the y‑component of the fundamental unit for the real quadratic
 * field with discriminant d, where d is square‑free and odd, d ≡ 3 (mod 4).
 */
static seq_t fundamental_unit_y(seq_t d)
{
    seq_t a0 = floor_sqrt(d);
    if (a0 * a0 == d)                /* d would be a perfect square – not expected */
        return 1;

    seq_t m = 0;
    seq_t dcoef = 1;                  /* denominator in the continued fraction */
    seq_t a = a0;

    seq_t p_prevprev = 0;
    seq_t p_prev = 1;
    seq_t q_prevprev = 1;
    seq_t q_prev = 0;

    for (;;) {
        seq_t p = a * p_prev + p_prevprev;
        seq_t q = a * q_prev + q_prevprev;

        seq_t lhs = seq_mul(p, p);
        seq_t rhs = seq_mul(d, seq_mul(q, q));

        if (lhs - rhs == 1)
            return q;                /* found minimal solution */

        /* next term of the continued fraction */
        m = dcoef * a - m;
        dcoef = (d - seq_mul(m, m)) / dcoef;
        a = (a0 + m) / dcoef;

        /* shift for next iteration */
        p_prevprev = p_prev;  p_prev = p;
        q_prevprev = q_prev;  q_prev = q;
    }
}

/*
 * Main sequence function.
 * For index n (0‑based), find the nth square‑free odd integer ≡ 3 (mod 4)
 * and return the y‑component of its fundamental unit.
 */
seq_t a(idx_t n)
{
    /* Locate the n‑th admissible discriminant */
    uint64_t idx = 0;              /* count of admissible numbers seen */
    uint64_t candidate = 3;        /* first odd integer ≡ 3 (mod 4) */

    while (idx < n) {
        candidate += 4;           /* next candidate ≡ 3 (mod 4) */
        if (is_squarefree(candidate))
            ++idx;
    }

    seq_t d = (seq_t)candidate;   /* discriminant to process */

    return fundamental_unit_y(d);
}
