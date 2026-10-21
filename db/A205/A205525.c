/*
 * A205525  Nonprime numbers k such that gcd(k, sigma(k)) == sigma(k) (mod k).
 * https://oeis.org/A205525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Nonprime numbers k such that gcd(k, sigma(k)) == sigma(k) (mod k).  
   This implementation searches successive integers, caches the terms, and
   returns the n‑th term (a(1)=1). */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    /* next integer to examine */
    static idx_t next_k = 1;

    /* expand cache until we have at least n terms */
    while (seq_len < n)
    {
        idx_t k = next_k++;
        /* skip primes */
        if (is_prime(k))
            continue;

        seq_t sigma = sigma_of(k);
        seq_t g = gcd((seq_t)k, sigma);

        /* condition: (sigma - g) % k == 0   <=>  sigma ≡ g (mod k) */
        if ((sigma - g) % (seq_t)k == 0)
        {
            /* ensure capacity */
            if (seq_len + 1 > seq_cap)
            {
                size_t new_cap = (seq_cap == 0) ? 1 : seq_cap << 1;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)k;
        }
    }
    return seq[n - 1];
}
