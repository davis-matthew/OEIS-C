/*
 * A212279  A002144(n+1)^2+1 mod A002144(n), where A002144 are the Pythagorean
 *          primes (p=4k+1).
 * https://oeis.org/A212279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n^2 + 1) mod p_{n+1} where p_i are the primes
 *            congruent to 1 (mod 4)
 * Range:     u8 n <= 14, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

/* Multiply two numbers modulo m without overflow (works for any unsigned seq_t). */
static seq_t mod_mul_safe(seq_t a, seq_t b, seq_t mod)
{
    seq_t result = 0;
    while (b) {
        if (b & 1) {
            result += a;
            if (result >= mod)
                result -= mod;
        }
        a += a;
        if (a >= mod)
            a -= mod;
        b >>= 1;
    }
    return result;
}

/* cache of Pythagorean primes (1 mod 4) */
static uint64_t *prime_list = NULL;   /* prime_list[i] = p_{i+1}   */
static size_t        prime_count = 0;  /* number of primes stored    */
static size_t        prime_cap   = 0;  /* capacity of prime_list    */
static uint64_t      candidate   = 5;   /* current odd candidate     */

/* Ensure that prime_list contains at least indices 0 … n (i.e. n+1 primes). */
static void ensure_primes(idx_t n)
{
    size_t target = (size_t)n;          /* last needed index (inclusive) */

    if (prime_count > target)
        return;                         /* already enough primes */

    /* Allocate or enlarge the table if necessary. */
    if (prime_cap < 8) {
        prime_cap = 8;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* Generate primes until we have (target+1) of them. */
    while (prime_count <= target) {
        if ((candidate & 3u) == 1u && is_prime_seq((seq_t)candidate)) {
            if (prime_count == prime_cap) {      /* grow array */
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(
                                 prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = candidate;
        }
        candidate += 2;                         /* squares left side  */
    }
}

seq_t a(idx_t n)
{
    /* We need p_n   (index n-1) and p_{n+1} (index n). */
    /* For n>=1 this is valid; behaviour for n=0 is irrelevant.        */
    ensure_primes(n);

    uint64_t pn      = prime_list[(size_t)(n - 1)]; /* p_n   */
    uint64_t pn_next = prime_list[(size_t)n];      /* p_{n+1} */

    seq_t mod = (seq_t)pn_next;
    seq_t sq  = mod_mul_safe((seq_t)pn, (seq_t)pn, mod); /* (p_n^2) % p_{n+1} */
    sq += 1;
    if (sq >= mod)
        sq -= mod;
    return sq;
}
