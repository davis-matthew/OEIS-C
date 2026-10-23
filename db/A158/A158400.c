/*
 * A158400  Primes of the form (k+1)^k - k^(k-1).
 * https://oeis.org/A158400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime of the form (k+1)^k - k^(k-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static state: list of found primes, capacity, next k to test */
    static seq_t *primes = NULL;
    static size_t primes_len = 0;
    static size_t primes_cap = 0;
    static idx_t next_k = 2;   /* start testing with k = 2 */

    /* Grow the list until we have at least n primes */
    while ((size_t)n > primes_len)
    {
        /* Compute (k+1)^k and k^(k-1) using the provided big integer routine */
        seq_t pow1 = big_pow((seq_t)(next_k + 1), (seq_t)next_k);
        seq_t pow2 = big_pow((seq_t)next_k, (seq_t)(next_k - 1));
        seq_t val  = pow1 - pow2;   /* (k+1)^k - k^(k-1) */

        /* If it is prime, store it */
        if (is_prime_big(val))
        {
            if (primes_len == primes_cap)
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                if (!tmp) abort();             /* allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = val;
        }
        ++next_k;
    }

    return primes[n - 1];   /* 0‑based index into the prime list */
}
