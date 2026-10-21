/*
 * A074170  Start with 1, add the next number if one gets a prime then add the
 *          next number else subtract the next...
 * https://oeis.org/A074170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1) = 1; for n > 1, if a(n‑1) == 1 or |a(n‑1)| is prime then
 *            a(n) = a(n‑1) + n, else a(n) = a(n‑1) – n
 * Range:     i8 n <= 20, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   abs_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/abs_seq_t.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    seq_t v = 1; /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        bool prime = false;
        if (v == 1)                /* treat the initial 1 as "prime" */
            prime = true;
        else
        {
            seq_t abs_v = abs_seq_t(v);
            /* is_prime_big considers only positive numbers; abs_v ≥ 2 here */
            prime = is_prime_big(abs_v);
        }

        if (prime)
            v = v + (seq_t)i;
        else
            v = v - (seq_t)i;
    }
    return v;
}
