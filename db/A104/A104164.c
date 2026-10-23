/*
 * A104164  Sophie Germain type primes where 5*Prime[n]=2*Prime[m]+1.
 * https://oeis.org/A104164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = (5*Prime[n]−1)/2 if that number is prime
   (the index n starts at 1) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* safety, sequence starts at n=1 */
        return 0;

    /* static state ----------------------------------------------------- */
    static seq_t *results = NULL;        /* stored sequence terms    */
    static size_t results_cap = 0;       /* capacity of the array   */
    static size_t results_cnt = 0;       /* number of terms stored  */
    static uint64_t next_prime_candidate = 2; /* next integer to test */

    /* Main loop: generate terms until we have at least n terms */
    while (results_cnt < n) {
        /* Find next prime p */
        uint64_t p = next_prime_candidate;
        while (!is_prime_uint64(p))
            ++p;
        next_prime_candidate = p + 1;     /* next candidate for following iteration */

        /* Compute candidate a = (5*p−1)/2 */
        seq_t candidate = ((seq_t)5 * (seq_t)p - 1) / 2;

        /* If candidate is prime, store it as the next sequence term */
        if (is_prime_uint64((uint64_t)candidate)) {
            /* grow array if needed */
            if (results_cnt >= results_cap) {
                size_t new_cap = results_cap ? results_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);   /* allocation failed */
                results = tmp;
                results_cap = new_cap;
            }
            results[results_cnt++] = candidate;
        }
    }

    return results[n - 1];
}
