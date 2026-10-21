/*
 * A051416  Primes whose digits are composite; primes having only {4, 6, 8, 9}
 *          as digits.
 * https://oeis.org/A051416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, max_seq_t_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051416: the n‑th prime whose decimal digits belong to {4,6,8,9}
   (equivalently, primes ending in 9 formed only with composite digits).
   The implementation uses a simple breadth‑first construction of all
   numbers of increasing length, testing each for primality with the
   helper function is_prime().  Primes found are cached to allow
   arbitrary ordering of calls. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/is_prime.h"
#include "../utils/max_seq_t_val.h"

/* Cache for found primes.  All values fit in seq_t when stored. */
static seq_t *prime_cache = NULL;
static size_t  prime_cnt  = 0;   /* number of primes stored */
static size_t  prime_cap  = 0;   /* current capacity of the cache */
static idx_t   processed_len = 0;/* longest length processed so far */

/* Append a prime to the cache, reallocating if necessary. */
static inline void add_prime(idx_t value)
{
    const uint64_t max_val = max_seq_t_val();
    if (value > max_val) return;    /* cannot fit in seq_t, skip */

    if (prime_cnt >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cap   = new_cap;
    }
    prime_cache[prime_cnt++] = (seq_t)value;
}

/* Recursively generate all numbers of a given length.
   Positions 0 .. len-2 may contain any of {4,6,8,9};
   the last position is forced to be 9.  Each finished number is
   tested for primality. */
static void gen_rec(idx_t cur, int pos, int len, const int *digits, size_t ndigits)
{
    if (pos == len) {
        if (is_prime(cur))
            add_prime(cur);
        return;
    }

    if (pos == len - 1) {              /* last digit must be 9 */
        gen_rec(cur * 10 + 9, pos + 1, len, digits, ndigits);
    } else {
        for (size_t i = 0; i < ndigits; ++i)
            gen_rec(cur * 10 + digits[i], pos + 1, len, digits, ndigits);
    }
}

/* Generate all numbers of a particular length and store primes found. */
static void generate_len(int len)
{
    static const int digits[4] = {4, 6, 8, 9};
    gen_rec(0, 0, len, digits, 4);
}

/* Return the n‑th term of the sequence.  Calls with n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* sequence is 1‑based; 0 is a safe default */

    while (prime_cnt < (size_t)n) {
        ++processed_len;
        generate_len((int)processed_len);
    }
    return prime_cache[(size_t)n - 1];
}
