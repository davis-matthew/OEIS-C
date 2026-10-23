/*
 * A201304  Primes of the form 10n^3+3.
 * https://oeis.org/A201304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime of the form 10 k³ + 3 for k ≥ 0. */
/* The value is found by brute‐force searching successive k. */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    idx_t count = 0;        /* how many primes found so far */
    idx_t k     = 0;        /* current k to test */
    seq_t value;

    while (count < n)
    {
        seq_t kk = (seq_t)k;
        value = kk * kk;          /* k² */
        value = value * kk;       /* k³ */
        value = value * (seq_t)10;/* 10 k³ */
        value = value + (seq_t)3; /* 10 k³ + 3 */

        if (is_prime_seq(value))
        {
            ++count;
            if (count == n)
                return value;
        }
        ++k;
    }
    return 0;   /* should never reach here for n ≥ 1 */
}
