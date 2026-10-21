/*
 * A077669  Triangle read by rows. Let k = n-th composite number. Then n-th
 *          row gives n smallest numbers unrelated to k (numbers which do not
 *          divide k, are not divisible by k and are not coprime to k).
 * https://oeis.org/A077669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, tabl
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   composite_at_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the pos‑th unrelated integer to the r‑th composite
 * where r is the smallest integer with r(r+1)/2 ≥ n
 * and pos = n – r(r-1)/2.
 * An integer m is *unrelated* to k if
 *   – m > 1,
 *   – gcd(m,k) > 1,
 *   – k is not a divisor of m  (k % m != 0),
 *   – m is not a multiple of k  (m % k != 0).
 * For the r‑th composite number k, the first r such unrelated integers
 * (ordered increasingly) form the r‑th row of the triangle.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/composite_at_r.h"

/* ---------- helper: gcd for uint64_t ---------- */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- helper: find the pos‑th unrelated integer to k ---------- */
static uint64_t find_unrelated(uint64_t k, idx_t pos)
{
    idx_t found = 0;
    for (uint64_t m = 2; ; ++m) {
        if (k % m == 0)
            continue;                /* m divides k */
        if (m % k == 0)
            continue;                /* k divides m */
        if (gcd_u64(k, m) > 1) {
            ++found;
            if (found == pos)
                return m;
        }
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* defined only for n ≥ 1 */

    /* find row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* position inside the row (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t pos = n - prev_tri;

    /* r‑th composite number */
    uint64_t k = composite_at_r(r);

    /* find the pos‑th unrelated number to k */
    uint64_t result = find_unrelated(k, pos);

    return (seq_t)result;
}
