/*
 * A229307  Numbers k such that A031971(2*k) <> k (mod 2*k).
 * https://oeis.org/A229307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A031971(2k) != k (mod 2k), where
 *            A031971(m) = Sum_{j=1..m} j^m. Computed by a search over k. The
 *            sum mod 2k is checked prime power by prime power: a closed form
 *            for odd primes and brute force for other prime powers. Terms are
 *            cached
 * Range:     u8 n <= 105, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9731 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A229307_pm(uint64_t b, uint64_t e, uint64_t q)
{
    uint64_t r = 1 % q;
    b %= q;
    while (e) {
        if (e & 1) r = (r * b) % q;
        b = (b * b) % q;
        e >>= 1;
    }
    return r;
}

/* returns 1 if k is in the sequence */
static int A229307_test(uint64_t k)
{
    uint64_t m = 2 * k;
    uint64_t rem = m;
    for (uint64_t p = 2; p * p <= rem || rem > 1; ) {
        if (p * p > rem) p = rem;
        if (rem % p == 0) {
            uint64_t q = 1;
            int a = 0;
            while (rem % p == 0) { rem /= p; q *= p; a++; }
            uint64_t T;
            if (a == 1 && p != 2) {
                T = ((m % (p - 1)) == 0) ? p - 1 : 0;
            } else {
                T = 0;
                for (uint64_t j = 1; j < q; ++j)
                    T = (T + A229307_pm(j, m, q)) % q;
            }
            uint64_t S = (((m / q) % q) * T) % q;
            if (S != k % q) return 1;
        }
        if (rem == 1) break;
        p += (p == 2) ? 1 : 2;
    }
    return 0;
}

static uint64_t *A229307_terms = 0;
static size_t A229307_cnt = 0, A229307_cap = 0;
static uint64_t A229307_next = 1;

seq_t a(idx_t n)
{
    while (A229307_cnt < (size_t)n) {
        if (A229307_test(A229307_next)) {
            if (A229307_cnt == A229307_cap) {
                A229307_cap = A229307_cap ? A229307_cap * 2 : 1024;
                A229307_terms = (uint64_t *)realloc(A229307_terms, A229307_cap * sizeof(uint64_t));
            }
            A229307_terms[A229307_cnt++] = A229307_next;
        }
        A229307_next++;
    }
    return (seq_t)A229307_terms[n - 1];
}
