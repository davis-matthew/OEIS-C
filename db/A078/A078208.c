/*
 * A078208  Smallest multiple of n beginning with the n-th prime.
 * https://oeis.org/A078208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest multiple of `n` whose decimal representation
 * starts with the n‑th prime.  For a given `n`, let `p = prime(n)`.
 * The algorithm examines successive powers of 10 and returns the first
 * multiple `n·k` that lies in the interval
 *   [ p·10ᵗ , (p+1)·10ᵗ – 1 ]
 * for some non‑negative integer `t`. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

/* static storage for the list of primes */
static idx_t *prime_arr = NULL;
static size_t prime_cnt = 0;
static bool prime_init = false;

/* ensure that prime_arr contains at least the first `n` primes */
static void ensure_primes(idx_t n)
{
    if (prime_init && n <= prime_cnt)
        return;

    /* a comfortable upper bound for the n‑th prime */
    idx_t limit = 200000;                /* covers n up to ~20000 */
    if (n > limit)
        limit = n * 20;                  /* rough estimate: p_n < 20n */

    if (prime_init) {
        free(prime_arr);
        prime_arr = NULL;
        prime_cnt = 0;
    }

    sieve_primes(limit, &prime_arr, &prime_cnt);
    prime_init = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; return 0 for safety */
        return (seq_t)0;

    ensure_primes(n);

    idx_t p = prime_arr[n - 1];
    seq_t prime_val      = (seq_t)p;
    seq_t prime_plus_one = prime_val + 1;
    seq_t pow10 = 1;                     /* 10ᵗ */

    seq_t seq_n = (seq_t)n;
    while (1) {
        seq_t lower = prime_val * pow10;
        seq_t upper = prime_plus_one * pow10 - 1;

        /* smallest k such that n·k ≥ lower */
        seq_t k = (lower + seq_n - 1) / seq_n;
        seq_t candidate = k * seq_n;

        if (candidate <= upper)
            return candidate;

        pow10 *= 10;                     /* try next digit length */
    }
}
