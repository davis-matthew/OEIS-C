/*
 * A109179  Eight-digit primes reverse of which is also prime such that each
 *          of decimal digits 0-to-7 used just once.
 * https://oeis.org/A109179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th smallest 8‑digit prime p that uses each digit
 *            0‑7 exactly once, such that rev(p) is also prime and also uses
 *            each digit 0‑7 exactly once. Only the smaller of each reversed
 *            pair is listed
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   next_permutation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_permutation.h"

/* ---------- helper functions ---------- */

/* modular multiplication (a*b) % mod using 128‑bit arithmetic */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation a^e % mod */
static uint64_t powmod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1) res = mulmod(res, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 0
    };
    for (size_t i = 0; small_primes[i]; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Bases that test all 64‑bit numbers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL, 0
    };
    for (size_t i = 0; bases[i]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* comparator for uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t*)a;
    uint64_t vb = *(const uint64_t*)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    /* static storage for pre‑computed terms */
    static bool ready = false;
    static uint64_t terms[35280];   /* ample space for all permutations */
    static size_t term_count = 0;

    if (!ready) {
        /* generate all qualifying primes once */
        int digits[8] = {0,1,2,3,4,5,6,7};
        do {
            /* skip numbers starting or ending with 0
               because rev(p) would not be 8‑digit */
            if (digits[0] == 0 || digits[7] == 0) continue;

            uint64_t val = 0, rev = 0;
            for (int i = 0; i < 8; ++i) {
                val = val * 10ULL + (uint64_t)digits[i];
            }
            for (int i = 7; i >= 0; --i) {
                rev = rev * 10ULL + (uint64_t)digits[i];
            }
            if (val < rev && isPrime64(val) && isPrime64(rev)) {
                terms[term_count++] = val;
            }
        } while (next_permutation(digits, 8));

        /* sort the terms ascending */
        qsort(terms, term_count, sizeof(uint64_t), cmp_uint64);
        ready = true;
    }

    if (n == 0 || (idx_t)n > term_count) return (seq_t)0;
    return (seq_t)terms[(size_t)n - 1];
}
