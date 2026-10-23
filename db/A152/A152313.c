/*
 * A152313  Primes without 0's or primes in their decimal expansion.
 * https://oeis.org/A152313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A152313: primes that contain only the digits 1,4,6,8,9 (no prime digits or 0) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/isPrimeGeneric.h"

/* Static storage for prime values found so far */
static seq_t *primes = NULL;          /* array of primes found                 */
static size_t primes_cnt = 0;         /* number of primes stored               */
static size_t primes_cap = 0;         /* current capacity of the array         */

/* Allowed digits (non‑prime digits except 0) */
static const uint8_t allowed_digits[5] = {1, 4, 6, 8, 9};

/* Ensure the primes array has space for “need” additional primes. */
static void ensure_primes_capacity(size_t need)
{
    if (primes_cnt + need <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 128;
    while (primes_cnt + need > new_cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    primes = tmp;
    primes_cap = new_cap;
}

/* Recursive generation of numbers with a fixed number of digits “len”.
   Any generated number that is prime is appended to the primes list. */
static void gen_rec(seq_t prefix, size_t pos, size_t len)
{
    if (pos == len) {
        if (isPrimeGeneric(prefix)) {
            ensure_primes_capacity(1);
            primes[primes_cnt++] = prefix;
        }
        return;
    }
    for (size_t i = 0; i < 5; ++i) {
        gen_rec(prefix * 10 + allowed_digits[i], pos + 1, len);
    }
}

/* Generate primes until we have at least “target” entries stored.  The
   function remembers the maximum length processed so far to avoid
   regenerating the same numbers on subsequent calls. */
static void generate_up_to(idx_t target)
{
    static size_t max_len_done = 0;   /* longest length processed so far */
    if (primes_cnt >= (size_t)target)
        return;

    size_t len = max_len_done + 1;
    while (primes_cnt < (size_t)target) {
        gen_rec(0, 0, len);
        ++len;
    }
    max_len_done = len - 1;           /* the last length that was processed */
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0; safe return */
    generate_up_to(n);
    return primes[n - 1];
}
