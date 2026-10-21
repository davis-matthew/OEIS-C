/*
 * A066984  a(n) = gcd(prime(n+1) - 1, prime(n) + 1).
 * https://oeis.org/A066984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(prime(n+1) - 1 , prime(n) + 1)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a dynamically resized array of prime numbers.
   It expands the table on demand until it contains at least the (n+1)‑th
   prime.  Casting from idx_t to seq_t is done explicitly to keep all
   arithmetic in the seq_t type.  The built‑in GCD routine from
   "../utils/gcd.h" is used. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static uint32_t *prime_table  = NULL;
static size_t    prime_count  = 0;   /* number of primes stored          */
static size_t    prime_capacity = 0; /* allocated capacity of prime_table */
static uint32_t  next_candidate  = 3; /* next odd number to test          */

/* Ensure the prime table contains at least target_index + 1 primes
   (0‑based indices).  Fills the table incrementally using trial division.  */
static void extend_primes_to(size_t target_index)
{
    if (prime_count == 0) {
        /* allocate initial capacity */
        if (prime_capacity == 0) {
            prime_capacity = 64;
            prime_table = (uint32_t*)malloc(prime_capacity * sizeof(uint32_t));
        }
        prime_table[prime_count++] = 2;  /* prime #1 */
    }

    while (prime_count <= target_index) {
        uint32_t cand = next_candidate;
        bool is_p = true;

        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t sq = (uint64_t)prime_table[i] * prime_table[i];
            if (sq > cand) break;
            if (cand % prime_table[i] == 0) {
                is_p = false;
                break;
            }
        }

        if (is_p) {
            if (prime_count == prime_capacity) {
                prime_capacity = prime_capacity ? prime_capacity * 2 : 64;
                prime_table = (uint32_t*)realloc(prime_table, prime_capacity * sizeof(uint32_t));
            }
            prime_table[prime_count++] = cand;
        }

        next_candidate += 2; /* test only odd numbers */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* safety, though the sequence is defined for n≥1 */

    /* prime(n)   -> prime_table[n-1]
       prime(n+1) -> prime_table[n]          */
    extend_primes_to((size_t)n);

    uint32_t p_n  = prime_table[n - 1];
    uint32_t p_np1 = prime_table[n];

    seq_t x = (seq_t)p_np1 - 1;  /* prime(n+1) - 1 */
    seq_t y = (seq_t)p_n + 1;    /* prime(n)   + 1 */

    return gcd(x, y);
}
