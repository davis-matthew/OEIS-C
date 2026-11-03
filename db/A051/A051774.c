/*
 * A051774  Contracted numbers.
 * https://oeis.org/A051774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A051774 – Contracted numbers.
 * A number n is contracted if for its distinct divisors d1<d2<…<dk there exists an i
 * (with 1 ≤ i ≤ k) such that d1+d2+…+d(i‑1) ≥ d(i).  The algorithm below
 * finds the n‑th contracted number by testing every integer in order.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return true iff m has the contracted property. */
static bool is_contracted(uint64_t m)
{
    /* Factorise m into primes: p^e.  We keep up to 64 prime factors. */
    struct { uint64_t p; int e; } fac[64];
    int faccnt = 0;
    uint64_t n = m;

    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            fac[faccnt].p = p;
            fac[faccnt].e = e;
            ++faccnt;
        }
    }
    if (n > 1) {                     /* remaining prime factor */
        fac[faccnt].p = n;
        fac[faccnt].e = 1;
        ++faccnt;
    }

    /* Enumerate all divisors (at most 256 for the sizes we need). */
    uint64_t divisors[256];
    size_t dcnt = 1;
    divisors[0] = 1;

    for (int idx = 0; idx < faccnt; ++idx) {
        uint64_t p = fac[idx].p;
        int e = fac[idx].e;
        size_t prev = dcnt;
        for (size_t i = 0; i < prev; ++i) {
            uint64_t val = divisors[i];
            for (int k = 1; k <= e; ++k) {
                val *= p;
                if (dcnt < 256) divisors[dcnt++] = val;
            }
        }
    }

    /* Sort divisors in ascending order (simple insertion sort). */
    for (size_t i = 1; i < dcnt; ++i) {
        uint64_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j-1] > key) {
            divisors[j] = divisors[j-1];
            --j;
        }
        divisors[j] = key;
    }

    /* Test the contracted condition. */
    unsigned __int128 sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        if (sum >= divisors[i]) return true;
        sum += divisors[i];
    }
    return false;
}

/* External function that returns the n-th contracted number. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1; safety */

    uint64_t count = 0;
    uint64_t candidate = 1;

    while (true) {
        if (is_contracted(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
