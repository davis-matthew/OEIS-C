/*
 * A121057  Triangle read by rows: T(n,m) = prime(n)^m*(prime(n) - 1)/2, 1 <=
 *          m <= n.
 * https://oeis.org/A121057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,m) = prime(n)^m*(prime(n)-1)/2 , read by rows
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS sequence A121057.
   It uses get_prime(k) to obtain the k‑th prime and pow_uint
   for exponentiation.  All arithmetic is performed in seq_t,
   with careful handling to avoid overflow for 64‑bit builds. */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Determine the row k such that (k-1)k/2 < n ≤ k(k+1)/2. */
    idx_t low = 1, high = n, k = 0;
    while (low <= high) {
        idx_t mid = (low + high) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n) {
            k = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    /* Position m within the row: 1 ≤ m ≤ k. */
    __int128 prev_t = (__int128)(k - 1) * k / 2;
    idx_t m = n - (idx_t)prev_t;

    /* Compute the term T(k,m). */
    uint32_t pk = get_prime(k);                       /* prime(k) */
    seq_t pow = pow_uint((uint64_t)pk, m);            /* pk^m */

    seq_t result;
    if (pk == 2) {                                    /* special case */
        result = pow / 2;                              /* (pk-1)/2 = 1/2 */
    } else {                                          /* pk odd, (pk-1)/2 is integer */
        result = pow * ((seq_t)((pk - 1) / 2));
    }
    return result;
}
