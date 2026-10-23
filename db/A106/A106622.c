/*
 * A106622  Primes of the form r(r(r(n)+1)+1)+1, where A141468(n)=r(n)=n-th
 *          nonprime.
 * https://oeis.org/A106622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find primes of the form r(r(r(n)+1)+1)+1,
   where r(k) is the k‑th nonprime (0,1,4,6,8,9,10,…).  The function
   a(n) returns the nth such prime for n ≥ 1.  The algorithm
   generates nonprimes on demand and stores the primes found so far
   in a memoised table.  This works for all supported seq_t sizes
   (uint8_t…etc) as long as the values stay within 64‑bit bounds. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

/* ---------- nonprime list (r(k)) ---------- */
static seq_t *nonprimes = NULL;   /* array of nonprime values      */
static idx_t  nonprimes_sz   = 0; /* number of stored nonprimes    */
static idx_t  nonprimes_cap  = 0; /* capacity of the array         */
static seq_t  next_nonprime_int = (seq_t)0; /* next integer to test */

/* Ensure the nonprime array has at least 'cap' slots. */
static void ensure_nonprimes_cap(idx_t cap)
{
    if (cap <= nonprimes_cap) return;
    idx_t newcap = nonprimes_cap ? nonprimes_cap * 2 : 1024;
    if (newcap < cap) newcap = cap;
    nonprimes = (seq_t *)realloc(nonprimes, newcap * sizeof(seq_t));
    nonprimes_cap = newcap;
}

/* Fill the nonprime array up to index 'k' (1‑based).  */
static void ensure_nonprimes_upto(idx_t k)
{
    if (k <= nonprimes_sz) return;
    while (nonprimes_sz < k) {
        if (!is_prime_uint64((uint64_t)next_nonprime_int)) {
            ensure_nonprimes_cap(nonprimes_sz + 1);
            nonprimes[nonprimes_sz++] = next_nonprime_int;
        }
        next_nonprime_int++;
    }
}

/* Return the k‑th nonprime (k ≥ 1). */
static inline seq_t r_of(idx_t k)
{
    ensure_nonprimes_upto(k);
    return nonprimes[k - 1];
}

/* ---------- prime list ------------------- */
static seq_t *primes = NULL;          /* array of found primes      */
static idx_t  primes_sz   = 0;        /* number of stored primes    */
static idx_t  primes_cap  = 0;        /* capacity of the array      */
static idx_t  last_m  = 1;            /* last tested index m        */

/* Ensure the primes array has at least 'cap' slots. */
static void ensure_primes_cap(idx_t cap)
{
    if (cap <= primes_cap) return;
    idx_t newcap = primes_cap ? primes_cap * 2 : 128;
    if (newcap < cap) newcap = cap;
    primes = (seq_t *)realloc(primes, newcap * sizeof(seq_t));
    primes_cap = newcap;
}

/* Check primality of a seq_t value that fits in 64 bits. */
static bool is_prime_seq(seq_t x)
{
    return is_prime_uint64((uint64_t)x);
}

/* Return the nth term of the sequence.  Calls are memoised. */
seq_t a(idx_t n)
{
    while (primes_sz < n) {
        /* Search for the next index m yielding a prime. */
        for (;;) {
            last_m++;
            seq_t r1 = r_of(last_m);
            idx_t idx2 = (idx_t)(r1 + (seq_t)1);
            seq_t r2 = r_of(idx2);
            idx_t idx3 = (idx_t)(r2 + (seq_t)1);
            seq_t r3 = r_of(idx3);
            seq_t p = r3 + (seq_t)1;

            if (is_prime_seq(p)) {
                if (primes_sz == 0 || primes[primes_sz - 1] != p) {
                    ensure_primes_cap(primes_sz + 1);
                    primes[primes_sz++] = p;
                }
                break;   /* prime found – move on to next required term */
            }
        }
    }
    return primes[n - 1];
}
