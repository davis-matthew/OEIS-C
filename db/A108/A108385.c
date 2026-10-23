/*
 * A108385  Primes p such that p's set of distinct digits is {3,7,9}.
 * https://oeis.org/A108385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   add_prime, isPrime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n‑th prime whose decimal digits are only 3, 7, 9
   and contain each of those digits at least once.  The primes are
   generated in increasing order and stored in a static array so that
   successive calls (in any order) are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Function prototypes from the utils library ------------------------------------
   add_prime: appends a prime to a dynamic uint64_t array.
   isPrime64 : Miller–Rabin primality test for 64‑bit integers (deterministic).
-------------------------------------------------------------------------------*/
#include "../utils/add_prime.h"    /* uint64_t **arr, size_t *cap, size_t *cnt, uint64_t val */
#include "../utils/isPrime64.h"    /* bool isPrime64(uint64_t n) */

/* Static state used by a() ------------------------------------------------------
   prime_buf   : dynamic array of found primes
   prime_cap   : allocated capacity of prime_buf
   prime_cnt   : number of primes stored
   curr_len    : current digit length being generated
--------------------------------------------------------------------------*/
static uint64_t *prime_buf   = NULL;   /* dynamic array of primes */
static size_t      prime_cap = 0;      /* allocated capacity of prime_buf */
static size_t      prime_cnt = 0;      /* number of primes stored */
static int         curr_len   = 1;     /* next length to generate */

/* Recursive traversal of all numbers with digits 3, 7, 9 of a given length.  */
static void
gen_len_rec(int pos, int len, uint64_t val, int mask)
{
    /* When all positions are filled, check the digits mask and primality. */
    if (pos == len) {
        if (mask == 7 && isPrime64(val)) {
            add_prime(&prime_buf, &prime_cap, &prime_cnt, val);
        }
        return;
    }

    /* Digits in ascending order – guarantees numeric order for a fixed length. */
    static const int digits[3] = {3, 7, 9};
    static const int bits[3]   = {1, 2, 4};

    for (int i = 0; i < 3; ++i) {
        gen_len_rec(pos + 1, len, val * 10 + digits[i], mask | bits[i]);
    }
}

/* Main sequence function ------------------------------------------------------
   Returns the (n‑1)th prime in the stored list.  If fewer than n primes
   have been found, the function generates more numbers until the count
   reaches n.  The function is safe for arbitrary order of calls.
--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, but harmless */

    /* Generate new primes until we have at least n of them. */
    while (prime_cnt < (size_t)n) {
        gen_len_rec(0, curr_len, 0, 0);   /* generate all numbers of current length */
        ++curr_len;                      /* next length will be processed next time */
    }

    /* Return the (n‑1)th stored prime.  Casting to seq_t is safe for all
       supported build types. */
    return (seq_t)prime_buf[(size_t)n - 1];
}
