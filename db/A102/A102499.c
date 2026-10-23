/*
 * A102499  Primes of the concatenated form 3nn3.
 * https://oeis.org/A102499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   digit_count, isprime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102499 – Primes of the form 3nn3.
   a(n) is the n‑th prime obtained by concatenating the decimal digits
   3, n, n, 3. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/isprime_big.h"   /* bool isprime_big(seq_t) */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t) */
#include "../utils/digit_count.h"

/* build the number 3 n n 3 for a given n (idx_t)   */
static seq_t build_candidate(idx_t n)
{
    idx_t d = digit_count(n);
    seq_t p1 = pow10((idx_t)(2 * d + 1));        /* 10^(2d+1)   */
    seq_t p2 = pow10((idx_t)(d + 1));            /* 10^(d+1)    */
    return (seq_t)((seq_t)3 * p1
                   + (seq_t)n * p2
                   + (seq_t)n * (seq_t)10
                   + (seq_t)3);
}

/* Return the nth term of the sequence (offset = 1) */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; value 0 is unused */
        return (seq_t)0;

    idx_t found = 0;
    idx_t candidate_n = 1;

    while (found < n)
    {
        seq_t cand = build_candidate(candidate_n);
        if (isprime_big(cand))
        {
            found++;
            if (found == n)
                return cand;
        }
        candidate_n++;
    }

    /* Should never be reached */
    return (seq_t)0;
}
