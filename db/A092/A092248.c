/*
 * A092248  Parity of number of distinct primes dividing n (function omega(n))
 *          parity of A001221.
 * https://oeis.org/A092248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A092248 – parity of the number of distinct prime factors of n.
    a(n) = 0 if ω(n) is even, 1 if ω(n) is odd (where ω(1)=0).    */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n == 0 the sequence is undefined, but return 0 to be safe. */
    if (n == (idx_t)0)
        return (seq_t)0;

    idx_t temp = n;
    seq_t parity = 0;            /* 0 = even, 1 = odd */

    /* Count each distinct prime factor once; flip parity each time. */
    for (idx_t p = (idx_t)2; p <= temp / p; ++p)
    {
        if (temp % p == (idx_t)0)
        {
            parity ^= (seq_t)1; /* toggle parity for this new prime factor */
            while (temp % p == (idx_t)0)
                temp /= p;     /* remove all copies of this prime */
        }
    }

    /* If a residual factor > 1 remains, it is prime and distinct. */
    if (temp > (idx_t)1)
        parity ^= (seq_t)1;

    return parity;
}
