/*
 * A078611  Radius of the shortest interval (of positive length) centered at
 *          prime(n) that has prime endpoints.
 * https://oeis.org/A078611
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest k>0 with prime(n)-k and prime(n)+k both prime *
 *            (k is the radius of the shortest interval centered at prime(n)
 *            having prime * endpoints). n<3 is undefined; the function
 *            returns 0 for such inputs * but the OEIS offset starts at 3. The
 *            implementation is O(gap) per * call, where gap...
 * Range:     u8 n <= 852, u16 n <= 9558, u32 n <= 9558, u64 n <= 9558,
 *            u128 n <= 9558, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n < 3) /* undefined below offset; return 0 as a placeholder */
        return (seq_t)0;

    /* prime(n) fits into 32 bits for n up to at least 10^7 */
    uint32_t p = get_prime(n);

    /* k must be even for the ends to stay odd (p is odd for n>=3). */
    for (uint32_t k = 2; ; k += 2) {
        if (p < k) /* safety guard – in practice a solution exists early */
            break;
        /* test primality of both endpoints */
        if (is_prime((idx_t)(p - k)) && is_prime((idx_t)(p + k)))
            return (seq_t)k;
    }
    return (seq_t)0; /* should never reach */
}
