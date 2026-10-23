/*
 * A117301  a(n) = prime(n+3)*prime(n) - prime(n+1)*prime(n+2).
 * https://oeis.org/A117301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n+3)*prime(n) - prime(n+1)*prime(n+2)
 * Range:     i8 n <= 10, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined below offset */

    /* obtain primes needed for the formula */
    unsigned long long p_n   = get_prime(n);
    unsigned long long p_n1  = get_prime(n + 1);
    unsigned long long p_n2  = get_prime(n + 2);
    unsigned long long p_n3  = get_prime(n + 3);

    /* compute with 64‑bit arithmetic to avoid overflow in 32‑bit builds */
    long long v1 = (long long)p_n3 * (long long)p_n;
    long long v2 = (long long)p_n1 * (long long)p_n2;

    return (seq_t)(v1 - v2);
}
