/*
 * A176279  Numbers k such that 2*prime(k)-1 and 2*prime(k)+1 are both prime
 *          or both composite.
 * https://oeis.org/A176279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176279: indices k for which 2*prime(k)-1 and 2*prime(k)+1 are both prime or both composite. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

/* the function returns the n-th term of the sequence, with a(1) = 1. */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
       Static storage for already computed terms.
       The array holds the indices k satisfying the required property.
       -------------------------------------------------------------------- */
    static seq_t *terms = NULL;      /* array of qualifying indices          */
    static size_t capacity = 0;      /* current allocated capacity (in terms) */
    static size_t term_count = 0;    /* number of terms stored              */
    static idx_t next_k = 1;         /* next index k to evaluate            */

    /* generate terms until we have at least n of them */
    while (term_count < n)
    {
        /* obtain the k-th prime (1‑based) */
        uint32_t p = get_prime(next_k);

        /* test 2*prime(k)-1 and 2*prime(k)+1 */
        uint64_t a1 = 2ULL * p - 1;
        uint64_t a2 = 2ULL * p + 1;
        bool prime1 = is_prime_small(a1);
        bool prime2 = is_prime_small(a2);

        if (prime1 == prime2)               /* both prime or both composite */
        {
            /* grow the array if necessary */
            if (term_count == capacity)
            {
                size_t new_cap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* out‑of‑memory: abort safely   */
                terms = tmp;
                capacity = new_cap;
            }
            terms[term_count++] = (seq_t)next_k;   /* store the qualifying k   */
        }

        ++next_k;
    }

    /* return the requested term (1‑based indexing) */
    return terms[n - 1];
}
