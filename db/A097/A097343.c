/*
 * A097343  Triangle read by rows in which row n gives Legendre symbol (k,p)
 *          for 0<k<=p where p = n-th prime.
 * https://oeis.org/A097343
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 12717, i16 n <= 12717, i32 n <= 12717, i64 n <= 12717,
 *            i128 n <= 12717, big any n
 * Validated: standard, pass, 121 terms through n = 7938 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Legendre symbol triangle, flattened by rows.
   a(n) = Legendre symbol (k,p) where p is the prime
   indexed by the row, and k is the position within that row.
   Row n >= 2: start with prime p = 3 (prime 2 row omitted).
   a(n) is defined for all n >= 2. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Return the next odd prime after n.  n is assumed to be an odd prime >= 3. */
static inline uint64_t next_prime_uint64(uint64_t n)
{
    uint64_t cand = n + 2;
    while (!is_prime((idx_t)cand))
        cand += 2;
    return cand;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2 */

    uint64_t m   = (uint64_t)n - 2;   /* zero‑based offset within the flattened triangle */
    uint64_t sum = 0;                /* cumulative number of elements before current row */
    uint64_t p   = 3;                /* start with the first row prime (p=3) */

    /* Move to the row that contains the m‑th element. */
    while (sum + p <= m) {
        sum += p;
        p = next_prime_uint64(p);
    }

    /* Position within the current row: 0‑based. */
    uint64_t pos = m - sum;
    uint64_t k   = pos + 1;          /* 1‑based k */

    if (k == p)                /* Legendre symbol (p,p) = 0 */
        return (seq_t)0;

    int s = jacobi_symbol(k, p); /* Legendre symbol (k,p), ±1 */
    return (seq_t)s;
}
