/*
 * A066341  Sum of distinct terms in n-th row of Fermat's triangle.
 * https://oeis.org/A066341
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*2^(omega(n)-1) - n, where omega(n) = A001221(n)
 * Range:     u8 n <= 83, u16 n <= 4619, u32 n <= 13737, u64 n <= 13737,
 *            u128 n <= 13737, big any n
 * Validated: standard, pass, 94 terms through n = 9623 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* Compute 2^exp for any exponent that may not fit in the bit width of seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    int omega = count_distinct_primes((uint64_t)n);
    seq_t pow = pow2((idx_t)(omega - 1));
    seq_t n_seq = (seq_t)n;
    return (n_seq + 1) * pow - n_seq;
}
