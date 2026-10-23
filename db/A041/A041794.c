/*
 * A041794  Numerators of continued fraction convergents to sqrt(418).
 * https://oeis.org/A041794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041794: a(n) = 67714*a(n-6) - a(n-12);
   Initial terms: a(0)=20, a(1)=41, a(2)=184, a(3)=3721,
   a(4)=15068, a(5)=33857, a(6)=1369348, a(7)=2772553,
   a(8)=12459560, a(9)=251963753, a(10)=1020314572, a(11)=2292592897. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 12 terms – they are needed as seed values for the recurrence */
    static const seq_t init[12] = {
        (seq_t)20,   (seq_t)41,   (seq_t)184,   (seq_t)3721,
        (seq_t)15068,(seq_t)33857,(seq_t)1369348,(seq_t)2772553,
        (seq_t)12459560,(seq_t)251963753,
        (seq_t)1020314572,(seq_t)2292592897
    };

    if (n < 12)
        return init[(size_t)n];           /* small n – no recurrence needed */

    /* ring buffer of the last 12 computed terms */
    seq_t ring[12];
    for (size_t i = 0; i < 12; ++i)
        ring[i] = init[i];

    /* iterate from 12 up to n, applying the linear recurrence */
    for (idx_t k = 12; k <= n; ++k) {
        seq_t val = (seq_t)67714 * ring[(k - 6) % 12] - ring[(k - 12) % 12];
        ring[k % 12] = val;
    }

    return ring[n % 12];
}
