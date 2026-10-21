/*
 * A079151  Primes p such that p-1 has at most 3 prime factors, counted with
 *          multiplicity; i.e., primes p such that bigomega(p-1) =
 *          A001222(p-1) <= 3.
 * https://oeis.org/A079151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1814, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, is_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079151: primes p such that bigomega(p-1) ≤ 3.
   The function builds the sequence lazily and remembers all found
   primes in a static dynamic array.  Calls in any order are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"           /* bool is_prime(idx_t x); */
#include "../utils/bigomega.h"           /* seq_t bigomega(idx_t n); */
#include "../utils/seq_alloc_new_array.h"/* seq_t* seq_alloc_new_array(size_t n); */
#include "../utils/seq_free_array.h"     /* void seq_free_array(seq_t* p); */

static seq_t *prime_list = NULL;   /* array of qualifying primes */
static size_t prime_list_len = 0;   /* number of stored primes        */
static size_t prime_list_cap = 0;   /* allocated capacity             */
static idx_t  last_checked   = 1;   /* largest integer already examined */

/* Append a new prime to the dynamic array, reallocating if necessary. */
static void append_prime(seq_t val)
{
    if (prime_list_len == prime_list_cap) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 16;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < prime_list_len; ++i)
            new_arr[i] = prime_list[i];
        if (prime_list)
            seq_free_array(prime_list);
        prime_list = new_arr;
        prime_list_cap = new_cap;
    }
    prime_list[prime_list_len++] = val;
}

/* Extend the prime list until it contains at least 'need' entries. */
static void ensure_primes(idx_t need)
{
    while (prime_list_len < (size_t)need) {
        idx_t candidate = last_checked + 1;
        while (!is_prime(candidate))
            ++candidate;
        last_checked = candidate;

        if (bigomega(candidate - 1) <= 3)
            append_prime(candidate);
    }
}

/* Main sequence function: returns the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, harmless fallback */
    ensure_primes(n);
    return prime_list[(size_t)n - 1];
}
