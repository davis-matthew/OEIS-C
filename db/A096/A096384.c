/*
 * A096384  a(n) is the area of the triangle with sides prime(n), prime(n+2)
 *          and prime(n+4), rounded down to the nearest integer.
 * https://oeis.org/A096384
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( P*(P-2p1)*(P-2p2)*(P-2p3) ) / 4 ) * where p1
 *            = prime(n), p2 = prime(n+2), p3 = prime(n+4), * P = p1+p2+p3.
 *            This is the integer part of the area of the * triangle with
 *            sides p1, p2, p3
 * Range:     u8 none, u16 n <= 4, u32 n <= 31, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* Retrieve the three primes. */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 2);
    seq_t p3 = (seq_t)get_prime(n + 4);

    /* Semi‑perimeter as a rational number with denominator 1/2.  The
     * exact Heron product simplifies to:  P * (P-2p1) * (P-2p2) * (P-2p3) / 16.
     * Thus sqrt(product) = sqrt(N) / 4 where N = P*(P-2p1)*(P-2p2)*(P-2p3). */
    seq_t P  = p1 + p2 + p3;
    seq_t f1 = P - (p1 << 1); /* P - 2*p1 */
    seq_t f2 = P - (p2 << 1); /* P - 2*p2 */
    seq_t f3 = P - (p3 << 1); /* P - 2*p3 */

    seq_t N = P * f1 * f2 * f3;          /* 64‑bit * 64‑bit *… fits in seq_t (128‑bit). */
    seq_t sqrtN = floor_sqrt(N);          /* floor of sqrt(N) */
    return sqrtN >> 2;                    /* divide by 4, integer truncation */
}
