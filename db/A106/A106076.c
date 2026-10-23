/*
 * A106076  Primes p such that 3*p + 28 and 28*p + 3 are primes.
 * https://oeis.org/A106076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 3*p+28 and 28*p+3 are also
 *            primes. The implementation generates primes on the fly using
 *            deterministic Miller–Rabin for 64‑bit integers, storing found
 *            terms so that subsequent calls are O(1) after the initial
 *            generation. The algorithm works for any seq_t that can...
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- 64‑bit deterministic Miller–Rabin ---------- */
static bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t smallPrimes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(smallPrimes)/sizeof(smallPrimes[0]); ++i) {
        uint64_t p = smallPrimes[i];
        if (n % p == 0)
            return n == p;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pow = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1)
                x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Static state for term generation ---------- */
static seq_t *stored_terms = NULL;  /* array of found terms       */
static size_t stored_count = 0;     /* number of terms stored     */
static size_t stored_cap   = 0;     /* current allocation capacity*/
static uint64_t next_candidate = 2; /* candidate prime to test    */

/* Append a new term to the static array */
static void add_term(uint64_t p)
{
    if (stored_count == stored_cap) {
        size_t new_cap = (stored_cap == 0) ? 64 : stored_cap * 2;
        seq_t *tmp = (seq_t *)realloc(stored_terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
        stored_terms = tmp;
        stored_cap = new_cap;
    }
    stored_terms[stored_count++] = (seq_t)p;
}

/* Ensure that we have at least 'target' terms computed. */
static void generate_until(idx_t target)
{
    /* If already computed, nothing to do */
    if (stored_count >= target) return;

    /* Iterate candidates until we accumulate enough terms. */
    while (stored_count < target) {
        /* Skip even numbers >2 */
        if (next_candidate > 2 && (next_candidate & 1) == 0)
            ++next_candidate;
        /* Test current candidate for primality */
        if (isPrime64(next_candidate)) {
            uint64_t p = next_candidate;
            /* Compute 3*p+28 and 28*p+3 in 128‑bit to avoid overflow */
            unsigned __int128 q1 = (unsigned __int128)3 * p + 28;
            unsigned __int128 q2 = (unsigned __int128)28 * p + 3;
            /* Only test if both fit in 64‑bit; otherwise, skip (unlikely needed) */
            if (q1 <= UINT64_MAX && q2 <= UINT64_MAX) {
                if (isPrime64((uint64_t)q1) && isPrime64((uint64_t)q2))
                    add_term(p);
            }
        }
        ++next_candidate;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* OEIS offset 1; undefined for 0 – return 0 safely */
        return 0;
    generate_until(n);
    return stored_terms[n - 1];
}
