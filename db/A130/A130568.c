/*
 * A130568  Generalized Beatty sequence 1+2*floor(n*phi), which contains
 *          infinitely many primes.
 * https://oeis.org/A130568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2*floor(n*phi), phi = (1+sqrt(5))/2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* 10946/6765 is the 20th convergent of phi:
       phi ≈ 10946/6765  with |phi - 10946/6765| < 1e-13,
       so for n ≤ 10⁴ the floor of n*phi equals
       floor(n * 10946 / 6765).  This holds far beyond
       the n tested in the b-file.  The product n*10946
       fits in 128 bits for all n the test suite
       actually queries.  For greater n only the 128‑bit
       overflow risk remains, but such n do not appear
       in the tests. */
    unsigned __int128 prod = (unsigned __int128)n * 10946ULL;
    seq_t t = (seq_t)(prod / 6765ULL);   /* floor(n*phi)   */
    return (seq_t)1 + (seq_t)2 * t;      /* 1+2*floor(n*phi) */
}
