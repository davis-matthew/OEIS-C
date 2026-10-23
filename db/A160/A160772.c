/*
 * A160772  Number of nodes (or order) of a graph model obtained using an
 *          automata scheme on sets of order prime(n) >= 5 and in which all
 *          not halt states are linked by arcs (edges).
 * https://oeis.org/A160772
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-2)*(prime(n)-1)+1
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 6543, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3  (a(3) is the first term) */

#include <stdint.h>
#include <stdlib.h>

/* Cached list of primes (1‑based index).  */
static uint64_t *prime_list      = NULL;      /* array of primes      */
static size_t  prime_count       = 0;         /* number of primes stored  */
static size_t  prime_capacity    = 0;         /* allocated capacity       */

/* Ensure that at least `need` primes are stored in prime_list.  */
static void
ensure_prime_cache(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;  /* already have enough */

    /* Grow capacity to at least `need`.  */
    if (prime_capacity < 16) prime_capacity = 16;
    while (prime_capacity < (size_t)need)
        prime_capacity <<= 1;                /* double */

    /* Reallocate storage.  Cast is needed for C++ compilation. */
    uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                        prime_capacity * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);           /* allocation failed */
    prime_list = tmp;

    /* Generate primes until we have `need` of them.  */
    uint64_t cand = (prime_count == 0) ? 2ULL : prime_list[prime_count - 1] + 1ULL;
    while (prime_count < (size_t)need) {
        int isprime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p)                /* p*p > cand */
                break;
            if (cand % p == 0) {
                isprime = 0;
                break;
            }
        }
        if (isprime) {
            prime_list[prime_count++] = cand;
        }
        ++cand;
    }
}

/* Return the n‑th prime (n ≥ 1).  */
static uint64_t
get_prime(idx_t n)
{
    if (n == 0) return 0; /* undefined, but hides call errors */
    ensure_prime_cache(n);
    return prime_list[n - 1];
}

/* Sequence function.  */
seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* undefined for the offset, behavior does not matter */

    /* obtain the n‑th prime; prime(1)=2, etc. */
    seq_t p = (seq_t)get_prime(n);

    /* compute (p-2)*(p-1)+1 in seq_t arithmetic */
    return (p - 2) * (p - 1) + 1;
}
