/*
 * A054535  Square array giving Ramanujan sum T(n,k) = c_n(k) = Sum_{m=1..n,
 *          (m,n)=1} exp(2 Pi i m k / n), read by antidiagonals upwards (n >=
 *          1, k >= 1).
 * https://oeis.org/A054535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, nice
 * Formula:   not stated
 * Range:     i8 n <= 9975, i16 n <= 9975, i32 n <= 9975, i64 n <= 9975,
 *            i128 n <= 9975, big any n
 * Helpers:   gcd64, mobius_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the entry of the Ramanujan‑sum array c_m(k)
   read by anti‑diagonals upwards.
   T(m,k) = c_m(k) = φ(m) * μ(m/gcd(m,k)) / φ(m/gcd(m,k))
   where φ is Euler’s totient and μ is the Möbius function. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"
#include "../utils/mobius_uint64.h"

/* Euclidean greatest‑common‑divisor for uint64_t. */

/* Euler’s totient φ(n) for n ≥ 1. */
static seq_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    seq_t result = n;
    uint64_t p = 2;
    while (p * p <= n) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
        ++p;
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety: offset starts at 1 */

    /* Find the anti‑diagonal number s: minimal s with s*(s+1)/2 ≥ n. */
    uint64_t s = 1;
    while (s * (s + 1) / 2 < n) ++s;

    /* Position inside that diagonal (1‑based). */
    uint64_t prev = (s - 1) * s / 2;   /* elements before this diagonal */
    uint64_t idx  = n - prev;          /* 1‑based index within diagonal */

    /* Map to (row, col) in the Ramanujan‑sum table. */
    uint64_t row = s - idx + 1;        /* n in the array */
    uint64_t col = idx;                /* k in the array */

    uint64_t g = gcd64(row, col);
    uint64_t d = row / g;              /* n / gcd(n,k) */

    seq_t mu = mobius_uint64(d);
    if (mu == 0) return 0;             /* μ(d)=0 ⇒ sum equals 0 */

    seq_t phi_row = phi_uint64(row);
    seq_t phi_d   = phi_uint64(d);

    /* Compute T(row,col) = φ(row) * μ(d) / φ(d). */
    return (phi_row * mu) / phi_d;
}
