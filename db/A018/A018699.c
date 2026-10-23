/*
 * A018699  Divisors of 880.
 * https://oeis.org/A018699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018699: the n-th divisor of 880 in ascending order. */
/* 880 = 2^4 * 5 * 11, so the number of divisors is (4+1)*(1+1)*(1+1)=20. */
/* The function builds the small list once and re‑uses it. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices out of bounds (n<1 or n>20). */
    if (n < 1 || n > 20)
        return 0;

    /* One-time generation of all divisors of 880. */
    static seq_t cache[20];
    static bool ready = false;

    if (!ready)
    {
        idx_t pos = 0;
        idx_t a_exp, b_exp, c_exp;

        /* Enumerate all exponent combinations. */
        for (a_exp = 0; a_exp <= 4; ++a_exp)
        {
            for (b_exp = 0; b_exp <= 1; ++b_exp)
            {
                for (c_exp = 0; c_exp <= 1; ++c_exp)
                {
                    seq_t val = ((seq_t)1 << a_exp);     /* 2^a_exp */
                    if (b_exp)
                        val *= (seq_t)5;                 /* 5^b_exp */
                    if (c_exp)
                        val *= (seq_t)11;                /* 11^c_exp */
                    cache[pos++] = val;
                }
            }
        }

        /* Sort the array of 20 divisors in ascending order. */
        for (idx_t i = 1; i < 20; ++i)
        {
            seq_t key = cache[i];
            idx_t j = i;
            while (j > 0 && cache[j - 1] > key)
            {
                cache[j] = cache[j - 1];
                --j;
            }
            cache[j] = key;
        }

        ready = true;
    }

    /* Convert 1-indexed n to 0-indexed array position. */
    return cache[n - 1];
}
