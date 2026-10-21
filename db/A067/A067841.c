/*
 * A067841  Numbers n such that n and prime(n) end with the same three digits.
 * https://oeis.org/A067841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that k % 1000 ==
 *            prime(k) % 1000 (prime(k) is the k‑th prime number); offset 1
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   grow_array_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/grow_array_uint64.h"

/* --- prime storage -------------------------------------------- */
static uint64_t *prime_arr   = NULL;   /* array of primes, 0‑based: prime_arr[0] = 2 */
static size_t   prime_used   = 0;
static size_t   prime_cap    = 0;
static uint64_t prime_start  = 2;     /* next candidate to test for primality */

/* --- sequence storage ------------------------------------------ */
static uint64_t *seq_arr     = NULL;   /* array of sequence terms (as uint64_t) */
static size_t   seq_used    = 0;
static size_t   seq_cap     = 0;
static uint64_t last_n_checked = 1;   /* next n to test for inclusion */

/* Ensure primes[0 .. need-1] are generated. */
static void ensure_primes_upto(size_t need)
{
    while (prime_used < need) {
        while (!is_prime(prime_start)) prime_start++;      /* find next prime candidate */
        grow_array_uint64(&prime_arr, &prime_cap, &prime_used, prime_used + 1);
        prime_arr[prime_used - 1] = prime_start;
        prime_start++;                                    /* next candidate */
    }
}

/* Ensure seq_arr[0 .. n-1] are filled (n terms). */
static void ensure_seq_upto(size_t n)
{
    while (seq_used < n) {
        /* test next n value */
        grow_array_uint64(&seq_arr, &seq_cap, &seq_used, seq_used + 1);
        uint64_t candidate = last_n_checked++;              /* k */
        /* get prime(k) */
        ensure_primes_upto(candidate);
        uint64_t prime_val = prime_arr[candidate - 1];
        if ((candidate % 1000) == (prime_val % 1000)) {
            /* record only if condition holds */
            seq_arr[seq_used - 1] = candidate;
        } else {
            /* condition failed; remove placeholder */
            seq_arr[seq_used - 1] = 0;
            seq_used--;                                  /* revert count */
        }
    }
}

/* --- public function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* out of range, return 0 */
    ensure_seq_upto((size_t)n);
    return (seq_t)seq_arr[n - 1];             /* cast uint64_t to seq_t */
}
