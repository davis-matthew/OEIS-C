/*
 * A083502  Smallest k such that n*(n+k) + 1 is an n-th power.
 * https://oeis.org/A083502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (x^n – 1)/n – n , where x is the least integer > 1 with
 *            x^n ≡ 1 (mod n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/is_prime.h"  /* bool is_prime(idx_t n);           */
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    if (n == 1)   /* a(1) = 1 by definition */
        return (seq_t)1;

    seq_t n_seq = (seq_t)n;

    /* For prime n the minimal x is n+1 (from Fermat). */
    if (is_prime(n))
    {
        seq_t x = n_seq + (seq_t)1;
        seq_t x_pow = big_pow(x, n_seq);
        seq_t res = (x_pow - (seq_t)1) / n_seq - n_seq;
        return res;
    }

    /* For composite n search x from 2 upward until x^n ≡ 1 (mod n). */
    for (idx_t x_int = 2; x_int <= n + 1; ++x_int)
    {
        seq_t x_seq = (seq_t)x_int;
        seq_t mod_res = mod_pow(x_seq, n, n_seq);
        if (mod_res == (seq_t)1)
        {
            seq_t x_pow = big_pow(x_seq, n_seq);
            seq_t res = (x_pow - (seq_t)1) / n_seq - n_seq;
            return res;
        }
    }

    /* The loop above always finds a solution (x = n+1 works), so this
       return is unreachable, but required for completeness. */
    return (seq_t)0;
}
