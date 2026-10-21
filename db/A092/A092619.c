/*
 * A092619  Numbers with property that number of prime digits is prime.
 * https://oeis.org/A092619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with prime number of prime digits */
#include <stdbool.h>

/* small prime test (used for counting digits) */
static bool is_prime_small(int x)
{
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

/* check whether a number has a prime number of prime digits */
static bool has_prime_digit_count(seq_t n)
{
    int cnt = 0;
    while (n > 0)
    {
        int d = (int)(n % 10);
        if (d == 2 || d == 3 || d == 5 || d == 7)
            ++cnt;
        n /= 10;
    }
    return is_prime_small(cnt);
}

/* external linkage function requested */
seq_t a(idx_t n)
{
    /* start searching from the smallest value that can satisfy the property */
    seq_t val = 22;          /* a(1) = 22 */
    idx_t found = 0;

    while (1)
    {
        if (has_prime_digit_count(val))
        {
            ++found;
            if (found == n)
                return val;
        }
        ++val;
    }
}
