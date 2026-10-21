/*
 * A206258  1/8 the number of 2 X 2 -n..n arrays with a 2 X 2 -n..n inverse,
 *          i.e., with determinant +-1.
 * https://oeis.org/A206258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * sum_{k=1..n} phi(k) - 3
 * Range:     u8 n <= 10, u16 n <= 163, u32 n <= 334, u64 n <= 334,
 *            u128 n <= 334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi(k) is Euler's totient function */

/* The function keeps a static cache of totients and their cumulative sum
   up to whenever the function is called with a larger n.  The algorithm
   uses a linear sieve (O(n)) to fill the tables.  The phi values are
   stored in uint64_t which is sufficient for all n ≤ 10 000, and the
   final value is converted to seq_t when returned.  The code is valid
   in C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t once we need it for final calculation.
       We keep the totient tables in uint64_t to avoid problems with
       arbitrary‑precision seq_t during allocation. */
    static idx_t max_n = 0;              /* highest n cached so far */
    static uint64_t *phi = NULL;         /* phi[1..max_n] */
    static uint64_t *phi_sum = NULL;     /* cumulative sum of phi */
    static idx_t *primes = NULL;         /* list of primes */
    static size_t primes_cnt = 0;        /* number of primes seen */

    /* Return 0 for n==0 (not used in the sequence). */
    if (n == 0) return (seq_t)0;

    /* If we need more entries, recompute from scratch. */
    if (n > max_n)
    {
        /* Reallocate arrays to new size n+1 (index 0 unused for phi). */
        uint64_t *new_phi = (uint64_t *)realloc(phi, (n + 1) * sizeof(uint64_t));
        uint64_t *new_phi_sum = (uint64_t *)realloc(phi_sum, (n + 1) * sizeof(uint64_t));
        idx_t *new_primes = (idx_t *)realloc(primes, (n + 1) * sizeof(idx_t));

        if (!new_phi || !new_phi_sum || !new_primes)
            return (seq_t)0; /* allocation failure; should not happen */

        phi = new_phi;
        phi_sum = new_phi_sum;
        primes = new_primes;

        /* Linear sieve for phi up to n. */
        bool *is_comp = (bool *)calloc(n + 1, sizeof(bool));
        if (!is_comp)
            return (seq_t)0; /* allocation failure */

        phi[1] = 1;
        primes_cnt = 0;

        for (idx_t i = 2; i <= n; ++i)
        {
            if (!is_comp[i])
            {
                primes[primes_cnt++] = i;
                phi[i] = i - 1;
            }
            for (size_t j = 0; j < primes_cnt; ++j)
            {
                idx_t p = primes[j];
                idx_t ip = i * p;
                if (ip > n)
                    break;
                is_comp[ip] = true;
                if (i % p == 0)
                {
                    phi[ip] = phi[i] * (idx_t)p;
                    break;
                }
                else
                {
                    phi[ip] = phi[i] * (idx_t)(p - 1);
                }
            }
        }
        free(is_comp);

        /* Build cumulative sum of phi. */
        phi_sum[0] = 0;
        for (idx_t i = 1; i <= n; ++i)
            phi_sum[i] = phi_sum[i - 1] + phi[i];

        max_n = n;
    }

    /* Compute a(n) = 8 * phi_sum[n] - 3. */
    seq_t result = (seq_t)(8ULL * phi_sum[n] - 3ULL);
    return result;
}
