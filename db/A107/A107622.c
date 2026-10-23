/*
 * A107622  Primes plus alternately 2 and 3.
 * https://oeis.org/A107622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + (3 - n % 2)
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static seq_t *prime_list = NULL;   /* array of primes */
static idx_t  prime_count = 0;     /* number of primes stored */
static size_t prime_cap   = 0;     /* allocated capacity */

/* Ensure at least 'needed' primes are available. */
static void extend_prime_table(idx_t needed)
{
    while (prime_count < needed)
    {
        seq_t candidate;
        if (prime_count == 0)
            candidate = 2;
        else
        {
            candidate = prime_list[prime_count - 1] + 1;
            if (candidate > 2 && (candidate % 2 == 0))
                candidate += 1;   /* start at next odd number */
        }

        /* Find the next prime starting at 'candidate'. */
        bool found = false;
        while (!found)
        {
            bool is_prime = true;
            for (idx_t i = 0; i < prime_count; ++i)
            {
                seq_t p  = prime_list[i];
                seq_t pp = p * p;
                if (pp > candidate)
                    break;
                if (candidate % p == 0)
                {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                found = true;
            else
                candidate += 2;   /* skip even numbers > 2 */
        }

        /* Store the found prime. */
        if (prime_count == prime_cap)
        {
            size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
            seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();                /* allocation failure */
            prime_list = tmp;
            prime_cap  = new_cap;
        }
        prime_list[prime_count++] = candidate;
    }
}

/* Compute a(n) for the OEIS sequence A107622. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for n=0 */
    extend_prime_table(n);                  /* ensure first n primes are ready */
    seq_t add = (n & 1) ? 2 : 3;            /* 2 for odd n, 3 for even n */
    return prime_list[n - 1] + add;
}
