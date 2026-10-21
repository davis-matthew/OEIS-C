/*
 * A225680  Denominators of phi(k)/k, as k runs through the squarefree numbers
 *          (A005117).
 * https://oeis.org/A225680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = A005117(n) / gcd(A005117(n), φ(A005117(n)))
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7612, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute GCD of two seq_t values using the Euclidean algorithm */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Check whether a 64‑bit integer is squarefree (trial division) */
static bool is_squarefree_uint64(uint64_t v)
{
    for (uint64_t d = 2; d * d <= v; ++d) {
        if (v % d == 0) {
            int cnt = 0;
            while (v % d == 0) {
                v /= d;
                ++cnt;
                if (cnt > 1) return false;
            }
        }
    }
    return true;
}

/* Compute φ(n) for a 64‑bit integer; result is seq_t */
static seq_t phi_uint64(uint64_t n)
{
    seq_t result = (seq_t)n;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            result = result - result / (seq_t)p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result = result - result / (seq_t)m;
    return result;
}

/* Return the n‑th square‑free integer (n ≥ 1), cached statically */
static seq_t nth_squarefree(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but harmless */

    static seq_t *sq = NULL;
    static size_t cnt = 0;
    static size_t cap = 0;

    if (!sq) {
        cap = 128;
        sq = (seq_t*)malloc(cap * sizeof(seq_t));
    }
    if (cnt < (size_t)n) {
        uint64_t candidate = (cnt == 0) ? 1 : (uint64_t)sq[cnt - 1] + 1;
        while (cnt < (size_t)n) {
            while (!is_squarefree_uint64(candidate)) ++candidate;
            sq[cnt++] = (seq_t)candidate;
            candidate = (uint64_t)sq[cnt - 1] + 1;
        }
    }
    return sq[n - 1];
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t k = nth_squarefree(n);
    seq_t phi_k = phi_uint64((uint64_t)k);
    seq_t g = gcd_seq(k, phi_k);
    return k / g;
}
