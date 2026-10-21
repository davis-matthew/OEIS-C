/*
 * A051101  Primes p such that x^64 = -2 has a solution mod p.
 * https://oeis.org/A051101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd64, has_solution_prime, mul64, pow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051101 – primes p such that the congruence x^64 ≡ –2 (mod p) has a solution.
   Computation: enumerate primes and test the condition
     (-2)^((p‑1)/gcd(64,p‑1)) ≡ 1 (mod p).
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd64.h"
#include "../utils/has_solution_prime.h"
#include "../utils/mul64.h"
#include "../utils/pow64.h"

/* 64‑bit modular multiplication using 128‑bit intermediate */

/* Euclidean gcd for 64‑bit integers */

/* Deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d·2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for 64‑bit integers */
    const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static uint64_t candidate = 2;      /* next number to test */

    if (n == 0) return 0;              /* offset is 1 – undefined below */

    /* Generate terms until we have at least n entries */
    while (terms_len < n) {
        uint64_t p = candidate;

        /* Advance candidate to next odd after 2 */
        if (candidate == 2) candidate = 3;
        else candidate += 2;

        if (!is_prime_64(p)) continue;
        if (!has_solution_prime(p)) continue;

        /* Grow storage if necessary */
        if (terms_len == terms_cap) {
            terms_cap = terms_cap ? terms_cap * 2 : 8;
            uint64_t *tmp = (uint64_t*)realloc(terms, terms_cap * sizeof(uint64_t));
            if (!tmp) exit(1);
            terms = tmp;
        }
        terms[terms_len++] = p;
    }
    return (seq_t)terms[n - 1];
}
