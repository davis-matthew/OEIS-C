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
 * Helpers:   modinv, tri_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/modinv.h"
#include "../utils/tri_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but harmless */
    idx_t k = tri_root(n);        /* minimal k with T(k)>=n */
    idx_t m = k + 1;              /* modulus */
    idx_t Tprev = (idx_t)(((unsigned __int128)(k - 1) * k) / 2);
    idx_t j = n - Tprev;          /* position in the row, 1..k */
    return modinv(j, m);          /* 0 if gcd(j,m)!=1 */
}
