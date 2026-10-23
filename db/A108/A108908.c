/*
 * A108908  Largest n-digit number coprime to n.
 * https://oeis.org/A108908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = largest n-digit number coprime to n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mod_pow, pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10_idx.h"
#include "../utils/mod_pow.h"

static const uint64_t SMALL_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

seq_t a(idx_t n)
{
    /* Compute 10^n - 1, which is the largest n‑digit number */
    seq_t p10 = pow10_idx(n);
    seq_t x0   = p10 - 1;          /* 10^n - 1 */

    /* Factor n into distinct primes -------------------------------------- */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[20];
    size_t   prime_cnt = 0;
    uint64_t t = nn;
    for (size_t i = 0; i < sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]); ++i) {
        uint64_t p = SMALL_PRIMES[i];
        if (p * p > t) break;
        if (t % p == 0) {
            primes[prime_cnt++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) {
        primes[prime_cnt++] = t;   /* remaining prime factor */
    }

    /* Compute forbidden residues r_p = (10^n - 1) mod p ------------------- */
    uint8_t residues[20];
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t mod10n = mod_pow(10, (uint64_t)n, p);
        residues[i] = (uint8_t)((mod10n + p - 1) % p);  /* x0 mod p */
    }

    /* Find the smallest k ≥ 0 such that k % p != residues[i] for all p ---- */
    uint64_t k = 0;
    for (;;) {
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if ((k % primes[i]) == residues[i]) {
                ok = false;
                break;
            }
        }
        if (ok) break;
        ++k;
    }

    /* Result is 10^n - 1 - k */
    return x0 - (seq_t)k;
}
