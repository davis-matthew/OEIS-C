/*
 * A031920  a(n) = prime(10*n-9).
 * https://oeis.org/A031920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(10*n-9)
 * Range:     u8 n <= 6, u16 n <= 655, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of found primes, zero‑based index */
static seq_t *prime_list   = NULL;
static size_t prime_count  = 0;
static size_t prime_cap    = 0;
static uint64_t next_prime = 2;

/* Ensure that the list contains at least `target` primes   */
/* (1‑based index, i.e. target ≥ 1).                          */
static void ensure_primes_up_to(idx_t target)
{
    /* target fits into a `size_t` for the purpose of the loop   */
    size_t needed = (size_t)target;
    while (prime_count < needed)
    {
        /* Skip even numbers greater than 2 */
        if (next_prime > 2 && (next_prime & 1) == 0) { ++next_prime; continue; }

        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i)
        {
            seq_t p = prime_list[i];
            uint64_t pp = (uint64_t)p;
            if (pp * pp > next_prime) break;
            if (next_prime % pp == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            if (prime_count >= prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (seq_t *)realloc(prime_list, prime_cap * sizeof(seq_t));
            }
            prime_list[prime_count++] = (seq_t)next_prime;
        }
        ++next_prime;
    }
}

seq_t a(idx_t n)
{
    /* compute the 1‑based index of the required prime */
    idx_t prime_index = 10*n - 9;
    ensure_primes_up_to(prime_index);
    return prime_list[(size_t)(prime_index - 1)];
}
