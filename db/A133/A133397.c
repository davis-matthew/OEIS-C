/*
 * A133397  Triangular array read by rows: T(n, k) = prime(n+1)^k *
 *          prime(n)^(k*(k-1)/2) with T(0,0) = 1.
 * https://oeis.org/A133397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,c) where * r = floor((sqrt(8*n+1) - 1)/2) (row
 *            index) * c = n - r*(r+1)/2 (column index) * T(r,c) =
 *            prime(r+1)^c * prime(r)^(c*(c-1)/2) * prime(1)=2, prime(2)=3, …
 *            (1‑based), T(0,0)=1. * This reads the triangular array row by
 *            row
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 26, u128 n <= 35,
 *            big any n
 * Helpers:   _floor_isqrt_u64, seq_mul, seq_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/sieve_first_n_primes.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    /* ----- Prime table -------------------------------------------------- */
    static bool primes_ready = false;
    static uint32_t *primes = NULL;
    static size_t primes_len = 0;

    if (!primes_ready) {
        /* Get a generous amount of primes (first 2000) – more than enough for
         * n up to at least 10000 (row index <= ~141). */
        const size_t NEED = 2000;
        size_t tmpcnt = 0;
        sieve_first_n_primes((idx_t)NEED, &primes, &tmpcnt);
        primes_len = tmpcnt;
        primes_ready = true;
    }

    /* ----- Row and column indices ------------------------------------- */
    idx_t t = n * 8 + 1;                    /* 8*n + 1  (no overflow for n <= 2^61) */
    idx_t r = (_floor_isqrt_u64(t) - 1) / 2;
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;                    /* column index (0 <= c <= r) */

    if (r == 0)                       /* T(0,0) = 1 */
        return (seq_t)1;

    /* ----- Primes for this row ---------------------------------------- */
    seq_t p_r   = (seq_t)primes[r - 1];  /* prime(r)  */
    seq_t p_r1  = (seq_t)primes[r];      /* prime(r+1) */

    /* ----- Compute the two powers ------------------------------------- */
    seq_t part1 = seq_pow(p_r1, c);            /* prime(r+1)^c            */
    idx_t exp2  = (c * (c - 1)) / 2;           /* exponent for prime(r)   */
    seq_t part2 = seq_pow(p_r, exp2);

    /* ----- Multiply the two parts ------------------------------------- */
    return seq_mul(part1, part2);
}
