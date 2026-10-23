/*
 * A199978  Nonprime numbers whose multiplicative digital root is 0.
 * https://oeis.org/A199978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th nonprime number whose multiplicative digital root is
 *            0
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for the n-th positive integer that is not prime and has a multiplicative digital
 * root equal to 0. The search proceeds sequentially over increasing integers and stops when the n-th such
 * integer has been found. It uses the provided digit_product and is_prime_big utilities. */

#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

/* Return true if the multiplicative digital root of x is 0. */
static inline bool mult_root_zero(seq_t x)
{
    while (1)
    {
        /* If x has become a single digit, the root is x itself. */
        if (x < (seq_t)10)
            return x == (seq_t)0;

        seq_t prod = digit_product(x);
        if (prod == (seq_t)0)
            return true;

        x = prod;
    }
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 2;                 /* start from the first positive integer > 1 */

    while (count < n)
    {
        seq_t val = (seq_t)num;

        if (!is_prime_big(val) && mult_root_zero(val))
        {
            ++count;
            if (count == n)
                return val;
        }

        ++num;
    }

    /* The function always returns inside the loop for a valid n. */
    return (seq_t)0;
}
