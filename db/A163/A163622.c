/*
 * A163622  Composite numbers such that the sum of its smallest digit and the
 *          largest digit is a prime.
 * https://oeis.org/A163622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A163622: Composite numbers such that the sum of its smallest digit
 *  and the largest digit is a prime (at least two different digits required). */
#include <stdbool.h>

/* Check whether a small integer (≤17) is prime */
static bool prime_sum(int s)
{
    /* primes up to 17: 2,3,5,7,11,13,17 */
    return s==2 || s==3 || s==5 || s==7 || s==11 || s==13 || s==17;
}

/* Determine if n (≥2) is composite by trial division */
static bool is_composite(idx_t n)
{
    if (n < 4) return false;                /* 2 and 3 are prime */
    for (idx_t i = 2; i <= n / i; ++i)
    {
        if (n % i == 0) return true;
    }
    return false;
}

/* Main function: returns the n-th element of the sequence */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 11;  /* start searching from 12 */
    while (true)
    {
        ++candidate;
        if (!is_composite(candidate)) continue;

        /* extract smallest and largest digits */
        idx_t temp = candidate;
        int min_digit = 9, max_digit = 0;
        while (temp > 0)
        {
            int d = temp % 10;
            if (d < min_digit) min_digit = d;
            if (d > max_digit) max_digit = d;
            temp /= 10;
        }
        if (min_digit == max_digit) continue;            /* same digit only */

        int sum = min_digit + max_digit;
        if (!prime_sum(sum)) continue;                    /* sum not prime */

        ++count;
        if (count == n)
            return (seq_t)candidate;
    }
}
