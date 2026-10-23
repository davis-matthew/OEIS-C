/*
 * A176041  Triangle read by rows: R(n, k) = 2^(2n - 2) mod prime(2k),
 *          1<=k<=n.
 * https://oeis.org/A176041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(2r-2) mod prime(2k), where (r,k) is the position of
 *            the n‑th term in the triangle read row by row
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"        /* uint32_t get_prime(idx_t) */
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n */
    idx_t low = 1, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum < n) {
            low = mid + 1;
        } else {
            r = mid;
            high = mid - 1;
        }
    }

    /* Compute column k within row r */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;            /* 1 <= k <= r */

    /* Evaluate 2^(2r-2) mod prime(2k) */
    idx_t prime_index = 2 * k;            /* 2k-th prime */
    seq_t mod = (seq_t)get_prime(prime_index);
    idx_t exp = 2 * r - 2;
    return mod_pow((seq_t)2, exp, mod);
}
