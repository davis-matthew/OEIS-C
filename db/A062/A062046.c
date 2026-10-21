/*
 * A062046  Sum of even numbers between consecutive primes.
 * https://oeis.org/A062046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 649, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062046: sum of even numbers between two consecutive primes.
   a(n) = (prime(n+1)^2 - prime(n)^2)/4
   = (prime(n+1)+prime(n))*(prime(n+1)-prime(n))/4
   = sum of all even integers strictly between prime(n) and prime(n+1).
   Implementation uses a simple incremental prime sieve and arithmetic
   progressions.  All calculations are performed in 64‑bit
   unsigned integers and the final result is cast to seq_t.  */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Prime storage – a dynamic array of 64‑bit integers.  The array is
   extended on demand so that prime(n) is available for any n requested.
   The extra 64‑bit storage avoids overflow during prime generation
   even when seq_t is small.  The final result is cast to seq_t.      */
static uint64_t *prime_array   = NULL;
static size_t   prime_count   = 0;     /* number of primes stored  */
static size_t   prime_capacity = 0;   /* current allocated slots   */

static void add_prime(uint64_t p)
{
    if (prime_capacity == 0) {
        prime_capacity = 16;
        prime_array = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
    } else if (prime_count >= prime_capacity) {
        prime_capacity *= 2;
        prime_array = (uint64_t *)realloc(prime_array,
                                          prime_capacity * sizeof(uint64_t));
    }
    prime_array[prime_count++] = p;
}

/* Ensure that at least "needed_idx" primes (1‑based) are stored. */
static void ensure_primes(idx_t needed_idx)
{
    static uint64_t next_candidate = 3;   /* test odd numbers only */

    /* First prime – 2 */
    if (prime_count == 0) {
        add_prime(2);
        next_candidate = 3;
    }

    /* Test successive candidates until we have enough primes. */
    while (prime_count <= (size_t)needed_idx) {
        uint64_t cand = next_candidate;
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_array[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
            add_prime(cand);

        next_candidate += 2;   /* skip even numbers */
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  The function is called in O(n) time since
   each new prime is generated only once.  The function works for
   arbitrary‑precision and fixed‑size builds, as long as the result
   fits in seq_t. */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) uses primes 2 and 3. */
    if (n < 1)
        return (seq_t)0;     /* undefined for n<1, return 0 */

    /* We need primes up to index n+1. */
    ensure_primes(n + 1);

    uint64_t p = prime_array[n - 1];   /* prime(n)   */
    uint64_t q = prime_array[n];      /* prime(n+1) */

    /* Compute the first and last even numbers between p and q. */
    uint64_t first_even = (p % 2 == 0) ? p + 2 : p + 1;
    uint64_t last_even  = (q % 2 == 0) ? q - 2 : q - 1;

    if (first_even > last_even)
        return (seq_t)0;   /* no even numbers between the primes */

    /* Count of even numbers: (last - first)/2 + 1. */
    uint64_t count = (last_even - first_even) / 2 + 1;

    /* Sum of an arithmetic progression: count * (first + last) / 2. */
    uint64_t sum = count * (first_even + last_even) / 2;

    return (seq_t)sum;
}
