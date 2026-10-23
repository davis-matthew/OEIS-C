/*
 * A177226  Triangle read by rows: T(n, k) = 2^(prime(n) - prime(k)) mod
 *          prime(n), 1 <= k <= n.
 * https://oeis.org/A177226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A177226: triangle read by rows
 * a(n) = 2^(prime(r) - prime(k)) mod prime(r)
 * where r = smallest integer such that r(r+1)/2 >= n
 * and k = n - (r-1)r/2  (the column index in row r)
 * 
 * The values are generated on demand; no hard‑coded table is used.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */

    /* find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n ) r++;

    /* compute column index k */
    idx_t offset = 1 + (r - 1) * r / 2;   /* first index in this row */
    idx_t k = n - offset + 1;             /* 1‑based column */

    /* obtain the r‑th and k‑th primes (uint32_t fits all required indices) */
    uint32_t p_r = get_prime(r);
    uint32_t p_k = get_prime(k);

    /* exponent for the power of two */
    uint64_t exp = (uint64_t)p_r - (uint64_t)p_k;

    /* compute the triangle entry */
    return mod_pow((seq_t)2, exp, (seq_t)p_r);
}
