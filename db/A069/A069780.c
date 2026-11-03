/*
 * A069780  a(n) = gcd(d(n^3), d(n)).
 * https://oeis.org/A069780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(d(n³), d(n)) where d(m) is the number of divisors of
 *            m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9885 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
static void tau_n_and_n3(idx_t n, seq_t *tau, seq_t *tau3)
{
    *tau  = 1;
    *tau3 = 1;

    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            *tau  *= (seq_t)(exp + 1);
            *tau3 *= (seq_t)(3 * exp + 1);
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        *tau  *= 2;
        *tau3 *= 4;
    }
}

/* ------------------------------------------------------------------ */
/* Euclidean gcd for the type seq_t – works for built‑in integers as   */
/* well as for arbitrary‑precision types that support % and assignment. */
/* ------------------------------------------------------------------ */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Helper to compute τ(n) and τ(n³) for a given n.                       */
/* All arithmetic is performed in seq_t to keep the intermediate       */
/* results within the type bounds.                                      */

/* ------------------------------------------------------------------ */
/* Public entry point required by the problem statement.               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for offset 1 */

    seq_t tau, tau3;
    tau_n_and_n3(n, &tau, &tau3);
    return gcd_seq_t(tau, tau3);
}
