/*
 * A102057  Triangle of modular inverses of b (mod m) for b = 1, ..., m-1,
 *          where 0 indicates no modular inverse exists.
 * https://oeis.org/A102057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the modular inverse (modulo m) of j, where k = ceil((-1
 *            + sqrt(1 + 8n)) / 2), m = k + 1, j = n - k*(k-1)/2. If j and m
 *            are not coprime, return 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 8574 (2026-11-01)
 * Helpers:   modinv_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/modinv_idx_to_seq.h"

static idx_t tri_root(idx_t n)
{
    if (n == 0) return 0;
    /* upper bound for k: solve k*(k+1)/2 >= n
       k ≈ (sqrt(8n+1)-1)/2, add a safety margin of +1 */
    uint64_t hi = (uint64_t)((sqrt(8.0 * n + 1.0) + 1.0) / 2.0) + 1;
    uint64_t lo = 0;
    while (lo < hi) {
        uint64_t mid = (lo + hi) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but harmless */
    idx_t k = tri_root(n);        /* minimal k with T(k)>=n */
    idx_t m = k + 1;              /* modulus */
    idx_t Tprev = (idx_t)(((unsigned __int128)(k - 1) * k) / 2);
    idx_t j = n - Tprev;          /* position in the row, 1..k */
    return modinv_idx_to_seq(j, m);          /* 0 if gcd(j,m)!=1 */
}
