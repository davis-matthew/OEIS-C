/*
 * A069213  a(n) = n-th positive integer relatively prime to n.
 * https://oeis.org/A069213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the OEIS sequence A069213:
 *  a(n) = the n‑th positive integer that is relatively prime to n.
 *  The method uses the fact that the coprime residues to n repeat every
 *  modulus n.  If φ = φ(n) is Euler's totient function, then
 *      k = (n‑1)/φ
 *      pos = n - k*φ          (1 ≤ pos ≤ φ)
 *      a(n) = k*n + r
 *  where r is the pos‑th coprime residue among 1,…,n–1.
 *  The function caches results for efficiency.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Function to compute gcd of two seq_t values. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: Euler's totient φ(n) for  n ≤ UINT64_MAX. */
static seq_t totient_uint64(idx_t n)
{
    uint64_t nn = n;
    uint64_t phi = nn;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            while (nn % p == 0) nn /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (nn > 1) phi = phi / nn * (nn - 1);
    return (seq_t)phi;
}

/* Cached array for computed terms. */
static seq_t *cache = NULL;
static bool *computed = NULL;
static idx_t cache_size = 0;

/* Helper to ensure cache large enough. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    idx_t new_size = n * 2 + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    bool *new_computed = (bool *)realloc(computed, new_size * sizeof(bool));
    if (!new_cache || !new_computed) {
        /* Allocation failed – terminate. */
        return;
    }
    cache = new_cache;
    computed = new_computed;
    for (idx_t i = cache_size + 1; i < new_size; ++i) computed[i] = false;
    cache_size = new_size - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined but safe */
    ensure_cache(n);
    if (computed[n]) return cache[n];

    /* Compute φ(n). */
    seq_t phi = totient_uint64(n);

    /* Determine k and position within the next period. */
    idx_t k = (n - 1) / (idx_t)phi;           /* integer division */
    idx_t pos = n - k * (idx_t)phi;           /* 1 ≤ pos ≤ φ */

    /* Find the pos‑th coprime residue mod n. */
    idx_t count = 0;
    idx_t residue = 1;
    for (idx_t r = 1; r < n; ++r) {
        if (gcd_seq_t((seq_t)r, (seq_t)n) == (seq_t)1) {
            ++count;
            if (count == pos) {
                residue = r;
                break;
            }
        }
    }

    /* Compute a(n) = k*n + residue. */
    seq_t result = (seq_t)k * (seq_t)n + (seq_t)residue;
    cache[n] = result;
    computed[n] = true;
    return result;
}
