/*
 * A011625  Legendre symbol (n,223).
 * https://oeis.org/A011625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) == n^111 (mod 223) (Legendre symbol (n,223))
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of OEIS sequence A011625.
   a(0) = 0, a(n) = 1 if n is a quadratic residue mod 223,
   a(n) = -1 if n is a non-residue, and a(223*k) = 0. */
seq_t a(idx_t n)
{
    /* Reduce n modulo 223 (the prime). */
    uint64_t base = n % 223ULL;
    uint64_t res = 1;
    unsigned exp = 111;          /* (223-1)/2 */
    while (exp) {
        if (exp & 1)
            res = (res * base) % 223ULL;
        base = (base * base) % 223ULL;
        exp >>= 1;
    }

    /* Convert the modular value to the signed output. */
    if (res == 1ULL)
        return (seq_t)1;
    if (res == 222ULL)               /* 222 == -1 mod 223 */
        return (seq_t)(-1);
    /* res == 0 corresponds to n divisible by 223 */
    return (seq_t)0;
}
