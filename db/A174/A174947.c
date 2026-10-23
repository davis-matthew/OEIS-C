/*
 * A174947  Triangle read by rows: T(n,k) = (prime(n)+1) mod prime(k).
 * https://oeis.org/A174947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) gives the n-th entry of the triangle T(r,c) = (prime(r)+1)
 *            mod prime(c) read by rows. The triangle is flattened in
 *            row-major order: r = min k such that k(k+1)/2 >= n c = n -
 *            k(k-1)/2 a(n) = (prime(r)+1) mod prime(c)
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* column index c */
    idx_t prev = (r * (r - 1)) / 2;
    idx_t c = n - prev;

    seq_t pr = (seq_t)get_prime(r);
    seq_t pc = (seq_t)get_prime(c);

    return (pr + 1) % pc;
}
