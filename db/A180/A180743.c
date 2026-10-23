/*
 * A180743  Ascending sequence of numbers such that the sum of any two
 *          distinct elements (even + odd) is a prime number.
 * https://oeis.org/A180743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically smallest lexicographically
 *            increasing sequence of positive integers such that every sum of
 *            a pair of distinct elements of different parity is prime. The
 *            sequence is built by appending the smallest integer greater than
 *            the current maximum that satisfies the prime‑sum...
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Static storage for the sequence and its parity sub‑arrays. */
    static seq_t *seq   = NULL;   /* all terms */
    static size_t seq_cap = 0, seq_len = 0;
    static seq_t *odd  = NULL;   /* odd terms only */
    static size_t odd_cap = 0, odd_cnt  = 0;
    static seq_t *even = NULL;   /* even terms only */
    static size_t even_cap = 0, even_cnt = 0;

    /* Return a previously computed value if possible. */
    if ((size_t)n <= seq_len)
        return seq[(size_t)n - 1];

    /* Grow the sequence until the requested index is reached. */
    while (seq_len < (size_t)n)
    {
        /* Candidate is the smallest integer larger than the current maximum. */
        seq_t cand = (seq_len == 0) ? 1 : seq[seq_len - 1] + 1;

        /* Find the smallest candidate satisfying the property. */
        for (;; ++cand)
        {
            bool ok = true;

            if ((cand & 1) == 0)            /* even candidate */
            {
                for (size_t i = 0; i < odd_cnt; ++i)
                {
                    seq_t sum = cand + odd[i];
                    if (!is_prime_big(sum))
                    {
                        ok = false;
                        break;
                    }
                }
            }
            else                            /* odd candidate */
            {
                for (size_t i = 0; i < even_cnt; ++i)
                {
                    seq_t sum = cand + even[i];
                    if (!is_prime_big(sum))
                    {
                        ok = false;
                        break;
                    }
                }
            }

            if (ok)
                break;   /* candidate accepted */
        }

        /* Append the accepted candidate to the full sequence. */
        grow_seq(&seq, &seq_cap, seq_len + 1);
        seq[seq_len++] = cand;

        /* Store the candidate in the appropriate parity array. */
        if ((cand & 1) == 0)              /* even */
        {
            grow_seq(&even, &even_cap, even_cnt + 1);
            even[even_cnt++] = cand;
        }
        else                              /* odd */
        {
            grow_seq(&odd, &odd_cap, odd_cnt + 1);
            odd[odd_cnt++] = cand;
        }
    }

    return seq[(size_t)n - 1];
}
