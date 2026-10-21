/*
 * A095244  a(n) = concatenation n,n-1,n-2,...,3,2,1 mod (prime(n)).
 * https://oeis.org/A095244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (concatenation of n, n-1, …, 1) mod prime(n)
 *
 * The function concatenates the decimal representations of the integers
 * n down to 1 and then takes the result modulo the nth prime.
 * It works for any seq_t (8,16,32,64  or 128 bit) and for all n >= 1.
 * The algorithm runs in O(n) time, with only 64-bit arithmetic needed for
 * intermediate values, since the modulus prime(n) is at most the
 * 10000th prime (< 10^6) for the required range.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 -- unused */
    seq_t prime = get_prime(n);           /* prime(n) */

    /* powers of 10 mod prime, up to 5 digits (10000 has 5 digits) */
    seq_t pow10[6];
    pow10[0] = 1 % prime;
    for (int d = 1; d <= 5; ++d)
        pow10[d] = (pow10[d-1] * 10) % prime;

    seq_t res = 0;
    for (idx_t k = n; ; --k) {
        int digits;
        if (k >= 10000)      digits = 5;
        else if (k >= 1000)  digits = 4;
        else if (k >= 100)   digits = 3;
        else if (k >= 10)    digits = 2;
        else                 digits = 1;

        res = (res * pow10[digits] + (seq_t)k) % prime;

        if (k == 1) break;
    }
    return res;
}
