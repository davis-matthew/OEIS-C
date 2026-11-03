/*
 * A160902  Square array read by antidiagonals: a(m,n) = the smallest prime >=
 *          m*n.
 * https://oeis.org/A160902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest prime >= (i+1)*(j+1) where (i,j) are
 * the 0‑based antidiagonal coordinates of n (n starts at 1).
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

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range */

    /* Find the diagonal d such that the d‑th triangular number
       Tri(d) = d*(d+1)/2 is >= n. */
    idx_t d = 1;
    idx_t tri = 1;                 /* Tri(1) */
    while (tri < n) {
        ++d;
        tri += d;                  /* Tri(d) = Tri(d-1)+d */
    }

    /* Triangular number of previous diagonal */
    idx_t tri_prev = tri - d;
    /* 0‑based offset inside the current diagonal */
    idx_t offset = n - tri_prev - 1;

    /* Coordinates (i,j) in 0‑based form: i = offset,
       j = d - offset - 1. */
    idx_t i = offset;
    idx_t j = d - offset - 1;

    /* Compute product m*n = (i+1)*(j+1). */
    seq_t prod = (seq_t)(i + 1) * (seq_t)(j + 1);

    /* Find the smallest prime >= prod. */
    seq_t candidate = prod;
    if (candidate < 2) candidate = 2;          /* handle prod == 1 */
    while (!is_prime_big(candidate))
        ++candidate;

    return candidate;
}
