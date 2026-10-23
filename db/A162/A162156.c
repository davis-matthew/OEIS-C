/*
 * A162156  Table which contains in row n the mapping of the n-th block of 4
 *          primes to 4 integers.
 * https://oeis.org/A162156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 33, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162156: 4‑by‑4 product combinations of consecutive primes.
   a(n) is the nth entry in row‑major order of matrix
   T(n,1), T(n,2), T(n,3), T(n,4).

   Terms are given by:
     c = prime(4n-3),  b = prime(4n-2),  m = prime(4n-1),  r = prime(4n)
   T(n,1) = b*m - c*r
   T(n,2) = c*m + b*r
   T(n,3) = b*m + c*r
   T(n,4) = c*m - b*r
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;               // undefined for n = 0, but we return 0
    idx_t row = (n - 1) / 4 + 1;        // 1‑based row index
    idx_t i   = (n - 1) % 4;            // 0‑based column               

    uint32_t p1 = get_prime(4 * row - 3);   // c
    uint32_t p2 = get_prime(4 * row - 2);   // b
    uint32_t p3 = get_prime(4 * row - 1);   // m
    uint32_t p4 = get_prime(4 * row);       // r

    switch (i) {
        case 0: // T(n,1)
            return (seq_t)p2 * p3 - (seq_t)p1 * p4;
        case 1: // T(n,2)
            return (seq_t)p1 * p3 + (seq_t)p2 * p4;
        case 2: // T(n,3)
            return (seq_t)p2 * p3 + (seq_t)p1 * p4;
        case 3: // T(n,4)
            return (seq_t)p1 * p3 - (seq_t)p2 * p4;
        default:
            return 0; // unreachable
    }
}
