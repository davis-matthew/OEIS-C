/*
 * A238257  Numbers n such that n and 2n+1 use only odd decimal digits.
 * https://oeis.org/A238257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 291, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238257: numbers n such that n and 2n+1 consist only of odd decimal 
   digits.  The function computes terms on demand by searching forward and 
   caching the results.  It works for any build where seq_t may be an unsigned 
   integer type, uint8_t/16/32/64/128, or an arbitrary‑precision integer class. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether all decimal digits of x are odd. */
static bool odd_digits(seq_t x)
{
    /* For any seq_t the operations below are defined by the requirements. */
    while (x != 0)
    {
        seq_t d = x % 10;      /* a single decimal digit as a seq_t */
        if ((d & 1) == 0)      /* even digit found */
            return false;
        x = x / 10;
    }
    return true;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    /* Static cache of computed terms. */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    /* Next integer to test. */
    static seq_t next_candidate = 1;

    /* Extend cache until it contains at least n terms. */
    while (seq_len < (size_t)n)
    {
        seq_t cur = next_candidate;

        if (odd_digits(cur))
        {
            seq_t twice_plus = cur * 2 + 1;   /* 2n+1 */
            if (odd_digits(twice_plus))
            {
                /* Grow storage if necessary. */
                if (seq_len == seq_cap)
                {
                    size_t new_cap = seq_cap ? seq_cap * 2 : 8;

#ifdef __cplusplus
                    seq_t *tmp = new seq_t[new_cap];
                    for (size_t i = 0; i < seq_len; ++i)
                        tmp[i] = seq[i];
                    delete[] seq;
#else
                    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
#endif
                    seq = tmp;
                    seq_cap = new_cap;
                }

                seq[seq_len++] = cur;   /* cache found term */
            }
        }

        ++next_candidate;
    }

    return seq[n - 1];
}
