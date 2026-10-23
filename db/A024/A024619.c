/*
 * A024619  Numbers that are not powers of primes p^k (k >= 0); complement of
 *          A000961.
 * https://oeis.org/A024619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = nth positive integer that is not a prime power (p^k,
 *            k>=0)
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 6613, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Check whether x is a power of a prime (including 1 = p^0). */
static bool is_prime_power(uint64_t x)
{
    if (x < 2)
        return true;  /* 1 is considered a prime power */

    uint64_t remaining = x;
    int distinct = 0;

    for (uint64_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            ++distinct;
            if (distinct > 1)
                return false;   /* more than one distinct prime factor */
            while (remaining % p == 0)
                remaining /= p;
        }
    }

    if (remaining > 1)  /* remaining is a prime factor */
        ++distinct;

    return distinct == 1;   /* exactly one distinct prime factor */
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* array of computed terms */
    static size_t seq_len = 0;     /* number of terms stored */
    static size_t seq_cap = 0;     /* allocated capacity */
    static uint64_t next_num = 2;  /* next natural number to test */

    if (n == 0)  /* offset starts at 1, safety check */
        return (seq_t)0;

    idx_t target = n - 1;          /* zero‑based index */

    if (target >= seq_len)
    {
        /* grow array if necessary */
        if (seq_len == seq_cap)
        {
            size_t new_cap = seq_cap ? seq_cap * 2 : 32;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0; /* allocation failed, return 0 */
            seq = tmp;
            seq_cap = new_cap;
        }

        /* generate terms until we have at least n terms */
        for (; seq_len <= (size_t)target; ++seq_len)
        {
            while (true)
            {
                if (!is_prime_power(next_num))
                {
                    seq[seq_len] = (seq_t)next_num;
                    ++next_num;
                    break;
                }
                ++next_num;
            }
        }
    }

    return seq[target];
}
