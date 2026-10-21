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
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

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
