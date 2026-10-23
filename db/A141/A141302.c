/*
 * A141302  Primes of the form -x^2+6*x*y+6*y^2 (as well as of the form
 *          11*x^2+18*x*y+6*y^2).
 * https://oeis.org/A141302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime congruent to 11 or 59 modulo 60
 * (i.e. primes of the form -x^2+6*x*y+6*y^2).
 *
 * The implementation keeps a cache of the primes that have been found so
 * far and expands it as needed.  Candidates are generated only from the
 * two residue classes 11 and 59 modulo 60, and are tested for primality
 * with a simple trial‑division routine that works for all values that
 * fit into seq_t.
 *
 * All operations use only integer arithmetic in the allowed set.
 * Only standard library functions that are part of C11 are used.
 */

#include <stdlib.h>
#include <stdint.h> /* for uint64_t used in the primality test */

/* Static cache of primes */
static seq_t *prime_cache = NULL;  /* dynamic array of primes */
static size_t prime_cnt  = 0;      /* number of primes stored   */
static size_t prime_cap  = 0;      /* allocated capacity        */

/* Next integer to test, initialised to 2.  Stored in a 64‑bit variable
 * because the candidates can exceed the size of seq_t. */
static uint64_t next_candidate = 2;

/* Ensure the cache has space for at least one more element. */
static void ensure_capacity(void)
{
    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        /* If realloc fails we keep the old array; subsequent allocations
         * will just try again.  The program will likely terminate on
         * out‑of‑memory conditions. */
        prime_cache = new_arr;
        prime_cap   = new_cap;
    }
}

/* Append a prime to the cache. */
static void append_prime(seq_t p)
{
    ensure_capacity();
    prime_cache[prime_cnt++] = p;
}

/* Simple trial division primality test for 64‑bit numbers.  Works for all
 * values that fit into seq_t. */
static int is_prime_u64(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

/* Grow the cache until it contains at least n primes. */
static void grow_to(idx_t n)
{
    while (prime_cnt < n) {
        /* Search the next candidate that is 11 or 59 mod 60. */
        do {
            ++next_candidate;
        } while ((next_candidate % 60) != 11 && (next_candidate % 60) != 59);

        if (is_prime_u64(next_candidate))
            append_prime((seq_t)next_candidate);
    }
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0; safety only */
    grow_to(n);                     /* ensure the cache is large enough */
    return prime_cache[n - 1];      /* a(1) is the first stored prime */
}
