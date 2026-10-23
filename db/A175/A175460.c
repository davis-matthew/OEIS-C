/*
 * A175460  Numbers n such that R(7*n) is not palindromic and R(7*n) is
 *          divisible by 7.
 * https://oeis.org/A175460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the n-th positive integer n such that:
 *   - n % 10 != 0
 *   - reverse(7*n) is not equal to 7*n  (i.e. 7*n is not palindromic)
 *   - reverse(7*n) is divisible by 7
 * This implements the OEIS sequence A175460.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the reverse function for seq_t. */
static inline seq_t reverse_seq(seq_t x)
{
    seq_t rev = 0;
    while (x > 0)
    {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

/* Cache storage for the sequence. */
static seq_t *seq_cache = NULL;      /* dynamic array of terms      */
static size_t seq_cap   = 0;         /* current allocated capacity  */
static size_t seq_len   = 0;         /* number of terms cached      */

/* Next integer to test for inclusion in the sequence. */
static idx_t candidate_n = 1;

/* Return the n‑th term of A175460 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n < 1; harmless. */

    /* Generate terms until we reach the requested index. */
    while (seq_len < (size_t)n)
    {
        /* Find the next qualifying integer. */
        while (true)
        {
            if (candidate_n % 10 == 0)   /* skip multiples of 10 */
            {
                ++candidate_n;
                continue;
            }

            seq_t m     = (seq_t)candidate_n * 7;
            seq_t revm  = reverse_seq(m);

            if (revm == m)           /* 7*n is palindromic → skip */
            {
                ++candidate_n;
                continue;
            }

            if (revm % (seq_t)7 != 0) /* reverse not divisible by 7 → skip */
            {
                ++candidate_n;
                continue;
            }

            /* Candidate satisfies all conditions. Store it. */
            if (seq_len == seq_cap)   /* grow the cache if needed */
            {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp)           /* allocation failed */
                    return 0;
                seq_cache = tmp;
                seq_cap   = new_cap;
            }

            seq_cache[seq_len++] = (seq_t)candidate_n;
            ++candidate_n;
            break;                  /* stop searching for this term */
        }
    }

    return seq_cache[(size_t)n - 1];
}
