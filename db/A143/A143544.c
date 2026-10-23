/*
 * A143544  Triangle read by rows, T(n,k) = 2 if n is prime, 1 otherwise;
 *          1<=k<=n.
 * https://oeis.org/A143544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 if the row index r such that r(r+1)/2 ≥ n is prime, 1
 *            otherwise
 * Range:     u8 n <= 9829, u16 n <= 9829, u32 n <= 9829, u64 n <= 9829,
 *            u128 n <= 9829, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

static inline idx_t find_row(idx_t n)
{
    unsigned __int128 sum = 0;
    idx_t r = 0;
    while (sum < n)
    {
        r++;
        sum += r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but handle gracefully */
    idx_t r = find_row(n);
    return is_prime(r) ? (seq_t)2 : (seq_t)1;
}
