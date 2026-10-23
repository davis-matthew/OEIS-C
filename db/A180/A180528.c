/*
 * A180528  Primes that become a different prime under the mapping 1 <=> 5.
 * https://oeis.org/A180528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 374, u32 n <= 595, u64 n <= 2200,
 *            u128 n <= 2200, big any n
 * Helpers:   ensure_capacity, is_prime, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A180528: primes that become a different prime when digits 1 and 5 are swapped.
   Search for primes p such that p ≠ map(p), both p and map(p) are prime.
   Sequence is indexed starting at 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"     /* bool is_prime(idx_t x);      */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t x);  */
#include "../utils/pow10.h"        /* seq_t pow10(idx_t e);        */
#include "../utils/ensure_capacity.h"

/* map_value swaps 1↔5 in the decimal representation of x. */
static seq_t map_value(seq_t x)
{
    if (x < 10) {
        /* single digit – just swap if necessary */
        if (x == 1) return 5;
        if (x == 5) return 1;
        return x;
    }

    /* count digits */
    idx_t digits = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        ++digits;
        tmp /= 10;
    }

    /* prepare 10^(digits-1) */
    seq_t power = pow10(digits - 1);
    seq_t result = 0;

    /* extract digits from most significant to least */
    while (x > 0) {
        seq_t digit = x / power;          /* most significant digit */
        seq_t rem   = x % power;          /* remainder */
        if (digit == 1) digit = 5;
        else if (digit == 5) digit = 1;
        result = result * 10 + digit;
        x      = rem;
        power  /= 10;                     /* move to next lower place */
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* undefined for n < 1 */

    static seq_t *results = NULL;   /* array of qualifying primes */
    static size_t count   = 0;      /* number of stored primes   */
    static size_t cap     = 0;      /* current allocation size  */
    static uint64_t cur    = 1;     /* last integer checked      */

    while (count < n) {
        ++cur;
        if (!is_prime(cur))                 /* skip non‑primes  */
            continue;

        seq_t mapped = map_value((seq_t)cur);
        if (mapped == (seq_t)cur)           /* mapping unchanged */
            continue;

        if (!is_prime_big(mapped))          /* mapped number not prime */
            continue;

        /* store the qualifying prime */
        ensure_capacity(count + 1, &cap, (void **)&results, sizeof(seq_t));
        results[count++] = (seq_t)cur;
    }

    return results[n - 1];   /* zero‑based index into the array */
}
