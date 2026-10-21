/*
 * A200146  Triangle read by rows: T(n, k) = mod(k^(n - 1), n), where 1 <= k <
 *          n.
 * https://oeis.org/A200146
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200146 (triangle read by rows):
   T(n,k) = k^(n-1) mod n, 1 <= k < n.
   The sequence a(n) is obtained by listing the triangle row‑wise, with
   a(2) as the first term (offset 2). */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* a(2) is the first term, so for n < 2 we return 0 (unused). */
    if (n < 2u)
        return 0;

    /* k is the 0‑based index in the list of all triangle entries. */
    idx_t k = n - 2u;

    /* Find the smallest r >= 2 such that r*(r-1)/2 > k.
       Use an integer square‑root to avoid overflow. */
    __int128 t = (__int128)1 + (__int128)8 * k;
    long double ld = sqrt((long double)t);
    idx_t sqrt_t = (idx_t)ld;

    /* Adjust because the long double conversion may round. */
    while ((__int128)sqrt_t * sqrt_t > t) --sqrt_t;
    while ((__int128)(sqrt_t + 1) * (sqrt_t + 1) <= t) ++sqrt_t;

    /* Initial candidate for r from the quadratic formula. */
    idx_t r = (sqrt_t + 1u) / 2u;

    /* Ensure the inequality holds.  Increment r if necessary. */
    while ((__int128)r * (r - 1u) / 2u <= k)
        ++r;

    /* Compute the offset within the row: j in [0, r-2]. */
    __int128 prev = (__int128)(r - 2u) * (r - 1u) / 2u;
    idx_t j = k - (idx_t)prev;           /* j is 0‑based within the row */

    seq_t base = (seq_t)(j + 1u);         /* column index k = j+1 */
    seq_t mod  = (seq_t)r;                /* modulus */
    idx_t exp  = r - 1u;                  /* exponent n-1 */

    /* Modular exponentiation: (base^exp) mod mod. */
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1u)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1u;
    }
    return result;
}
