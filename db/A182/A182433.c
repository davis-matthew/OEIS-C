/*
 * A182433  Smallest number such that the next n integers each have the square
 *          of one of the first n primes as a factor in order.
 * https://oeis.org/A182433
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by Chinese Remainder Theorem:
   a(n) is the smallest integer such that for k = 1..n,
   a(n)+k is divisible by the square of the k‑th prime. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

/* modular inverse of a modulo m (both < 2^64), m prime to a */
static uint64_t modinv_uint64(uint64_t a, uint64_t m) {
    /* extended Euclid on 64-bit integers */
    __int128 t0 = 0, t1 = 1;
    uint64_t r0 = m, r1 = a % m;
    while (r1 != 0) {
        uint64_t q = r0 / r1;
        uint64_t r2 = r0 - q * r1;
        r0 = r1; r1 = r2;
        __int128 new_t = t0 - (__int128)q * t1;
        t0 = t1; t1 = new_t;
    }
    if (r0 != 1) return 0;               /* no inverse */
    if (t0 < 0) t0 += m;
    return (uint64_t)t0;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;      /* undefined for n < 2, but callers ignore */

    /* obtain the first n primes */
    uint32_t *primes = NULL;
    size_t cnt;
    sieve_first_n_primes(n, &primes, &cnt);
    if (cnt < n) { free(primes); return 0; }

    seq_t x = 0;  /* current solution */
    seq_t M = 1;  /* product of processed moduli */

    for (idx_t i = 1; i <= n; ++i) {
        uint64_t p = primes[i - 1];
        uint64_t m_k = p * p;                /* modulus = prime^2 */
        seq_t r_k = ((seq_t)p * (seq_t)p) - (seq_t)i;  /* remainder = prime^2 - i */

        if (i == 1) {        /* initialization */
            x = r_k;
            M = (seq_t)m_k;
            continue;
        }

        /* solve x + t*M ≡ r_k (mod m_k)  →  t ≡ (r_k - x) * M⁻¹ (mod m_k) */
        uint64_t M_mod_m = (uint64_t)(M % (seq_t)m_k);
        uint64_t inv = modinv_uint64(M_mod_m, m_k);

        uint64_t x_mod = (uint64_t)(x % (seq_t)m_k);
        uint64_t r_mod = (uint64_t)(r_k % (seq_t)m_k);
        uint64_t diff = (r_mod + m_k - x_mod) % m_k;

        /* t = (diff * inv) mod m_k */
        seq_t t_seq = ((seq_t)diff * (seq_t)inv) % (seq_t)m_k;
        seq_t delta = t_seq * M;
        x += delta;

        M *= (seq_t)m_k;   /* update modulus */
    }

    free(primes);
    return x;
}
