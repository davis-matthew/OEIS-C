/*
 * A095745  Primes p for which A037888(p) = 3, i.e., primes whose binary
 *          expansion needs flips of just three bits to become palindrome.
 * https://oeis.org/A095745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   bitlen_uint64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A095745: primes whose binary expansion needs exactly 3 bit flips
   to become a palindrome.  The function enumerates primes, checks the
   bit‑flip condition, and caches the results so that any n‑th term can
   be returned in O(1) after the initial search. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/next_prime.h"   /* next_prime(uint64_t) → next prime ≥ start */
#include "../utils/bitlen_uint64.h"/* bitlen_uint64(uint64_t) → number of bits needed */

/* static data used for memoisation */
static seq_t *seq_primes = NULL;   /* array of found sequence values            */
static size_t seq_len   = 0;       /* number of values stored                    */
static size_t seq_cap   = 0;       /* capacity of the array                      */
static uint64_t next_start = 2;    /* next candidate for primality test          */

/* grow the array when needed */
static void ensure_capacity(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 8;
#ifdef __cplusplus
        /* allocate using C++ new, copy existing elements, then delete old array */
        seq_t *new_arr = new seq_t[new_cap];
        for (size_t i = 0; i < seq_len; ++i) {
            new_arr[i] = seq_primes[i];
        }
        delete[] seq_primes;
        seq_primes = new_arr;
#else
        /* allocate using C realloc, casting is required */
        seq_t *tmp = (seq_t *)realloc(seq_primes, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);             /* allocation failure */
        seq_primes = tmp;
#endif
        seq_cap = new_cap;
    }
}

/* return true iff prime p requires exactly 3 bit flips to become a binary palindrome */
static bool three_bit_palindrome(uint64_t p)
{
    int mismatches = 0;
    uint64_t len = bitlen_uint64(p);
    for (uint64_t i = 0; i < len / 2; ++i) {
        int low  = (p >> i) & 1;
        int high = (p >> (len - 1 - i)) & 1;
        if (low != high) {
            ++mismatches;
            if (mismatches > 3) return false;
        }
    }
    return mismatches == 3;
}

/* returns the n-th element of A095745 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* sequence defined for n≥1 */

    size_t target = (size_t)n;

    /* generate primes until we have enough sequence terms */
    while (seq_len < target) {
        uint64_t p = next_prime(next_start);
        next_start = p + 1;
        if (three_bit_palindrome(p)) {
            ensure_capacity();
            seq_primes[seq_len++] = (seq_t)p;
        }
    }

    return seq_primes[target - 1];
}
