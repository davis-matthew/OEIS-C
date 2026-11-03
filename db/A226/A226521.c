/*
 * A226521  Triangle read by rows: T(n,k) = smallest prime == k (mod n) if
 *          gcd(k,n)=1, otherwise 0, for n >= 2, 1 <= k < n.
 * https://oeis.org/A226521
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   On input n ≥ 2, find the row r of the triangle and the column k
 *            (1 ≤ k < r). If gcd(k,r) = 1, return the smallest prime p with p
 *            ≡ k (mod r); otherwise return 0. (A226521: read the triangle
 *            T(n,k) row‐wise.)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 105 terms through n = 106 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Compute gcd for unsigned 64‑bit integers. */
static idx_t
gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t
a(idx_t n)
{
    /* Off‑by‑one offset: the first listed term is a(2).  Values
       for n < 2 are irrelevant, so return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Find the smallest r such that the cumulative number of
       terms up to row r (inclusive) is at least n‑1. */
    idx_t r = 2;
    while (((r - 1) * r) / 2 < n - 1)
        ++r;

    /* Compute the position k within this row (1 ≤ k < r). */
    idx_t prev_terms = ((r - 2) * (r - 1)) / 2;   /* terms before row r */
    idx_t k = (n - 1) - prev_terms;              /* 1‑based column index */

    /* If k and r are not coprime, the triangle entry is 0. */
    if (gcd_u64(k, r) != 1)
        return (seq_t)0;

    /* Search for the smallest prime p ≡ k (mod r). */
    seq_t p = (seq_t)k;
    if (p < (seq_t)2)
        /* Bring the starting value to the least positive congruent
           integer ≥ 2.  Adding r (converted to seq_t) preserves
           the congruence class. */
        p += (seq_t)r;

    for (;; p += (seq_t)r)
        if (is_prime_big(p))
            break;

    return p;
}
