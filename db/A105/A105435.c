/*
 * A105435  Primes which with a 1 prepended remain prime.
 * https://oeis.org/A105435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1011, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A105435 – primes p such that the decimal number formed by
   prepending a 1 to p (i.e. 1·10^digits(p)+p) is also prime.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>            /* for memset */
#include "../utils/is_prime.h"  /* provides bool is_prime(uint64_t) */

/* The following list contains the first few primes; used for a quick
   divisibility filter that skips most composite candidates before the
   expensive is_prime call.  */
static const uint64_t small_primes[] = {
    3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
    37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
    79, 83, 89, 97
};
static const size_t num_small_primes = sizeof(small_primes) / sizeof(small_primes[0]);

seq_t a(idx_t n)
{
    /* static state for memoised computation */
    static seq_t *seq      = NULL;    /* array of found terms               */
    static idx_t seq_cap   = 0;       /* allocated capacity                 */
    static idx_t seq_len   = 0;       /* number of stored terms             */
    static uint64_t next   = 3;       /* next odd number to test            */

    if (n == 0) return 0;            /* value for n = 0 is undefined       */

    /* Generate terms until the nth one is available */
    while (seq_len < n) {
        uint64_t p = next;
        next += 2;                       /* skip even numbers                */

        /* Quick filter: skip numbers divisible by a small prime
           (except when the number itself is that prime).   */
        bool maybe_prime = true;
        for (size_t i = 0; i < num_small_primes; ++i) {
            uint64_t sp = small_primes[i];
            if (p == sp) { maybe_prime = true; break; }
            if (p % sp == 0) { maybe_prime = false; break; }
        }
        if (!maybe_prime) continue;

        /* Check that p is actually prime.  */
        if (!is_prime(p)) continue;

        /* Compute 10^digits(p). */
        uint64_t power10 = 1, t = p;
        while (t > 0) {
            power10 *= 10;
            t /= 10;
        }

        /* Concatenated number 1*10^digits(p) + p.  */
        uint64_t concat = power10 + p;
        if (!is_prime(concat)) continue;   /* concatenated number not prime */

        /* Store the qualifying prime. */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 16;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!seq) abort();             /* allocation failure            */
        }
        seq[seq_len++] = (seq_t)p;
    }

    return seq[n - 1];
}
