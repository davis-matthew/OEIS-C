/*
 * A178445  a(n) = prime(n)^2 mod n^2.
 * https://oeis.org/A178445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 mod n^2
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the nth term of OEIS A178445.
   It uses the helper get_prime(idx_t k) to obtain the k‑th prime.
   All arithmetic that could overflow the final seq_t type is
   performed in a 128‑bit unsigned integer if available, and the
   modulo operation is carried out on that wider type before
   casting back to seq_t.  This guarantees exactness for all
   values that fit in seq_t and works for the arbitrary‑precision
   build as well. */

#include <stdint.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    /* Convert n to a wider type to avoid intermediate overflow. */
#if defined(__SIZEOF_INT128__) || (defined(__GNUC__) && !defined(_MSC_VER))
    __uint128_t N = ( __uint128_t )n;
    __uint128_t n2 = N * N;                 /* n^2 */

    /* Retrieve the nth prime (1‑based). */
    uint32_t p32 = get_prime(n);
    __uint128_t P = ( __uint128_t )p32;
    __uint128_t p2 = P * P;                 /* p^2 */

    /* Compute the result modulo n^2 and cast back to seq_t. */
    return (seq_t)( p2 % n2 );
#else
    /* Fallback when 128‑bit integers are not available.
       For practical problem sizes the following 64‑bit
       arithmetic is sufficient.  If seq_t is smaller than
       64 bits, the result may overflow; this implementation
       is only required to be exact for values that fit in
       seq_t. */
    uint64_t N = (uint64_t)n;
    uint64_t n2 = N * N;
    uint32_t p32 = get_prime(n);
    uint64_t p2 = (uint64_t)p32 * (uint64_t)p32;
    return (seq_t)(p2 % n2);
#endif
}
