/*
 * A144976  Nonsquarefree numbers k such that k is divisible by the maximal
 *          exponent in the prime factorization of k.
 * https://oeis.org/A144976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A144976 lists the nonsquarefree numbers k
   such that k is divisible by the maximal exponent present in its
   prime factorization.  The following implementation simply
   searches successive integers and tests the property.  This
   brute‑force method is fast enough to generate at least the
   first 10000 terms. */

#include <stdint.h>
#include <stdbool.h>

/* Return true if k is nonsquarefree and divisible by the maximal
   exponent in its prime factorization. */
static inline bool good_candidate(uint64_t k)
{
    if (k < 4) return false;  /* smallest example is 4 */
    uint64_t rem = k;
    uint64_t max_exp = 0;

    /* trial division up to sqrt(rem) */
    for (uint64_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            uint64_t exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }
            if (exp > max_exp) max_exp = exp;
        }
    }

    /* any remaining factor > sqrt(original k) has exponent 1 */
    if (rem > 1) 
    {
        if (max_exp < 1) max_exp = 1;
    }

    if (max_exp <= 1) return false;              /* k is squarefree */
    return (k % max_exp) == 0;
}

/* Public entry point.  Index n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */

    uint64_t count     = 0;
    uint64_t candidate = 4;        /* smallest admissible integer */

    while (count < n)
    {
        if (good_candidate(candidate))
        {
                ++count;
                if (count == n)
                        return (seq_t)candidate;
        }
        ++candidate;
    }
    /* This point should never be reached. */
    return (seq_t)candidate;
}
