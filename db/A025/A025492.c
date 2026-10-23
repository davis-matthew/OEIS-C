/*
 * A025492  Fixed point reached by iterating the Kempner function A002034
 *          starting at n.
 * https://oeis.org/A025492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point reached by iterating the Kempner function A002034 starting at n */

#include <stdint.h>

/* Return the smallest k such that n | k! for n>1 */
static seq_t min_k_for_prime(seq_t p, seq_t e)
{
    seq_t low = 1, high = e * p;
    if (high < 1) high = 1;           /* safety guard for e==0 */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        seq_t cnt = 0;
        seq_t q = mid;
        while (q != 0) {
            q /= p;
            cnt += q;
        }
        if (cnt >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Kempner function A002034: smallest m such that n divides m! */
static seq_t kempner(seq_t n)
{
    if (n <= 1)
        return 1;
    seq_t result = 0;
    seq_t temp   = n;

    for (seq_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            seq_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t k = min_k_for_prime(p, exp);
            if (k > result)
                result = k;
        }
    }

    if (temp > 1) {                   /* remaining prime factor */
        seq_t k = min_k_for_prime(temp, 1);
        if (k > result)
            result = k;
    }
    return result;
}

/* A025492: fixed point of iterated Kempner function */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    while (1) {
        seq_t y = kempner(x);
        if (y == x)
            return y;
        x = y;
    }
}
