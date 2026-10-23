/*
 * A103270  a(n) = (prime(n)+prime(n+k)) mod 4, where k =
 *          (prime(n+1)-prime(n))/2.
 * https://oeis.org/A103270
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)+prime(n+k)) mod 4, where k =
 *            (prime(n+1)-prime(n))/2
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)               /* offset is 2; behaviour below is irrelevant */
        return (seq_t)0;

    /* prime(n), prime(n+1), prime(n+k) */
    seq_t p1 = get_prime(n);          /* prime(n)   */
    seq_t p2 = get_prime(n + 1);      /* prime(n+1) */

    /* k = (prime(n+1)-prime(n))/2 */
    seq_t diff_half = (p2 - p1) / (seq_t)2;
    idx_t k = (idx_t)diff_half;       /* k is always an integer */

    seq_t p3 = get_prime(n + k);      /* prime(n+k) */

    /* (prime(n)+prime(n+k)) mod 4 */
    return (p1 + p3) % (seq_t)4;
}
