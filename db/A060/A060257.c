/*
 * A060257  Numbers k such that 1/prime(k) has period prime(k) - 1.
 * https://oeis.org/A060257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 2620, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = index of the n‑th full‑reptend prime
   (primes p for which 10 is a primitive root modulo p). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static idx_t lastChecked = 2;                 /* next candidate to test */
    static idx_t *primes = NULL;                  /* list of primes found */
    static size_t primeCnt = 0, primeCap = 0;
    static idx_t *fullIdx = NULL;                 /* indices of qualifying primes */
    static size_t fullCnt = 0, fullCap = 0;

    /* ensure we have enough qualifying indices */
    while (fullCnt < n) {
        /* find next prime */
        idx_t p;
        for (p = lastChecked;; ++p) {
            if (p < 2) continue;
            bool prime = true;
            for (size_t i = 0; i < primeCnt; ++i) {
                idx_t q = primes[i];
                if (q > p / q) break;   /* q*q > p */
                if (p % q == 0) { prime = false; break; }
            }
            if (prime) break;           /* p is prime */
        }

        /* add p to the primes list */
        if (primeCnt == primeCap) {
            size_t newCap = primeCap ? primeCap * 2 : 16;
            primes = (idx_t *)realloc(primes, newCap * sizeof(idx_t));
            primeCap = newCap;
        }
        primes[primeCnt++] = p;
        lastChecked = p + 1;
        idx_t pos = primeCnt;           /* 1‑based index of p */

        /* test whether 10 is a primitive root mod p (p > 5) */
        if (p > 5) {
            idx_t m = p - 1;
            idx_t factors[64];
            size_t fCnt = 0;
            idx_t temp = m;
            for (size_t j = 0; j < primeCnt && primes[j] <= temp / primes[j]; ++j) {
                idx_t q = primes[j];
                if (temp % q == 0) {
                    factors[fCnt++] = q;
                    while (temp % q == 0) temp /= q;
                }
            }
            if (temp > 1) factors[fCnt++] = temp;

            bool ok = true;
            for (size_t j = 0; j < fCnt && ok; ++j) {
                idx_t exp = (p - 1) / factors[j];
                seq_t base = 10 % p;
                seq_t res = 1 % p;
                while (exp) {
                    if (exp & 1) res = (unsigned __int128)res * base % p;
                    base = (unsigned __int128)base * base % p;
                    exp >>= 1;
                }
                if (res == 1) ok = false;
            }
            if (ok) {
                if (fullCnt == fullCap) {
                    size_t newCap = fullCap ? fullCap * 2 : 16;
                    fullIdx = (idx_t *)realloc(fullIdx, newCap * sizeof(idx_t));
                    fullCap = newCap;
                }
                fullIdx[fullCnt++] = pos;
            }
        }
    }
    return (seq_t)fullIdx[n - 1];
}
