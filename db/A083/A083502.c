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
 * Validated: standard, pass, 36 terms through n = 382 (2026-11-01)
 * Helpers:   ipow, is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n == 1)   /* a(1) = 1 by definition */
        return (seq_t)1;

    seq_t n_seq = (seq_t)n;

    /* For prime n the minimal x is n+1 (from Fermat). */
    if (is_prime(n))
    {
        seq_t x = n_seq + (seq_t)1;
        seq_t x_pow = ipow(x, n_seq);
        seq_t res = (x_pow - (seq_t)1) / n_seq - n_seq;
        return res;
    }

    /* For composite n search x from 2 upward until x^n ≡ 1 (mod n). */
    for (idx_t x_int = 2; x_int <= n + 1; ++x_int)
    {
        seq_t x_seq = (seq_t)x_int;
        seq_t mod_res = powmod(x_seq, n, n_seq);
        if (mod_res == (seq_t)1)
        {
            seq_t x_pow = ipow(x_seq, n_seq);
            seq_t res = (x_pow - (seq_t)1) / n_seq - n_seq;
            return res;
        }
    }

    /* The loop above always finds a solution (x = n+1 works), so this
       return is unreachable, but required for completeness. */
    return (seq_t)0;
}
