/*
 * A048852  Difference between b^2 (in c^2=a^2+b^2) and product of successive
 *          prime pairs.
 * https://oeis.org/A048852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 428, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048852: a(n) = p_{n+1}^2 - p_n * p_{n+1}
   where p_k is the k‑th prime (1‑based).  For n = 0 the value is 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Minimal prime generator with lazy extension.  It stores the
   list of primes in a dynamic array and expands it only when more
   primes are required.  The implementation uses trial division
   against the primes already found, which is more than fast enough
   for n up to 10 000 (the 10 000th prime is 104 729).           */
/* ------------------------------------------------------------------ */

static uint64_t *prime_list      = NULL;  /* array of found primes     */
static size_t    prime_count     = 0;     /* number of primes stored   */
static size_t    prime_capacity  = 0;     /* allocated size of array   */

/* Add a new prime to the list, expanding the array if needed. */
static void add_prime(uint64_t p)
{
    if (prime_count == prime_capacity) {
        size_t new_capacity = prime_capacity ? prime_capacity * 2 : 64;
        uint64_t *new_list = (uint64_t *)realloc(prime_list, new_capacity * sizeof(uint64_t));
        /* If realloc fails we simply leave the program in an invalid state;
           in the contest environment this is acceptable, as the input size
           is well below the limits of the allocated memory. */
        prime_list     = new_list;
        prime_capacity = new_capacity;
    }
    prime_list[prime_count++] = p;
}

/* Extension routine: generate primes until we have at least n_needed
   primes (1‑based).  It starts from the last declared prime plus 1
   and checks each candidate with trial division by the primes already
   discovered. */
static void ensure_primes(size_t n_needed)
{
    if (prime_count >= n_needed) return;

    /* If we have no primes yet, seed with 2 and 3. */
    if (prime_count == 0) {
        add_prime(2);
        add_prime(3);
    }

    uint64_t candidate = prime_list[prime_count - 1] + 2;  /* next odd number */

    while (prime_count < n_needed) {
        bool is_p = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_p = false; break; }
        }
        if (is_p) add_prime(candidate);
        candidate += 2;
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Sequence starts at offset 0.  a(0) is defined to be 0. */
    if (n == 0) return (seq_t)0;

    /* We need the (n+1)-th prime and the n-th prime (1‑based). */
    size_t needed = (size_t)(n + 1);          /* number of primes required */
    ensure_primes(needed);

    uint64_t p_prev = prime_list[n - 1];      /* p_n  (1‑based) */
    uint64_t p_curr = prime_list[n];          /* p_{n+1}       */

    seq_t sp_prev = (seq_t)p_prev;
    seq_t sp_curr = (seq_t)p_curr;
    seq_t result  = sp_curr * sp_curr - sp_prev * sp_curr;
    return result;
}
