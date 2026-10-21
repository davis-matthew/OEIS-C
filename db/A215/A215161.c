/*
 * A215161  Primes congruent to {2, 3, 5, 7, 11} mod 17.
 * https://oeis.org/A215161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the list of primes p such that p ≡ 2,3,5,7 or 11 (mod 17).
   The function returns the n‑th element by incremental prime generation
   using trial division. */

#include <stdbool.h>
#include <stdlib.h>

/* Grow a dynamically resized array of idx_t.  The array pointer is updated
   to the new storage area.  The function works for C and C++. */
static void grow_array(idx_t *size, idx_t **arr, idx_t needed)
{
    if (*size >= needed) return;
    idx_t new_size = (*size == 0) ? 16 : (*size << 1);
    while (new_size < needed) new_size <<= 1;
    *arr = (idx_t*)realloc(*arr, new_size * sizeof(idx_t));
    *size = new_size;
}

/* Return the n‑th prime congruent to 2,3,5,7 or 11 modulo 17. */
seq_t a(idx_t n)
{
    /* static data retained between calls */
    static idx_t *all_primes      = NULL;        /* all primes found so far   */
    static idx_t *special_primes  = NULL;        /* primes with the desired mod */
    static idx_t all_count        = 0;
    static idx_t special_count    = 0;
    static idx_t all_cap          = 0;
    static idx_t special_cap      = 0;
    static idx_t candidate        = 2;           /* number currently being tested */

    /* Generate primes until we have at least n special ones */
    while (special_count < n)
    {
        /* Test whether 'candidate' is prime */
        bool is_prime = true;
        for (idx_t i = 0; i < all_count &&
                all_primes[i] <= candidate / all_primes[i]; ++i)
        {
            if (candidate % all_primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* Store the prime in the list of all primes */
            grow_array(&all_cap, &all_primes, all_count + 1);
            all_primes[all_count++] = candidate;

            /* Check the congruence condition */
            switch (candidate % 17)
            {
                case 2: case 3: case 5: case 7: case 11:
                    grow_array(&special_cap, &special_primes, special_count + 1);
                    special_primes[special_count++] = candidate;
                    break;
                default:
                    break;
            }
        }

        /* Advance to the next candidate: 2 → 3, then skip evens */
        if (candidate == 2) candidate = 3;
        else                candidate += 2;
    }

    /* Return the n‑th special prime (1‑based index) */
    return (seq_t)special_primes[n - 1];
}
