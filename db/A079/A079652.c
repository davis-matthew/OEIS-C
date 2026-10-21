/*
 * A079652  Prime numbers using only the curved digits 0, 3, 6, 8 and 9.
 * https://oeis.org/A079652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Computes a(n) = the n-th prime whose decimal representation uses
 *            only the digits 0,3,6,8,9 (intersection of A000040 and A072960)
 * Range:     u8 n <= 3, u16 n <= 115, u32 n <= 45, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* Digit list in ascending order; leading digit cannot be 0. */
static const uint8_t DIGITS[5] = {0, 3, 6, 8, 9};

static seq_t *CACHE = NULL;          /* Array holding computed terms */
static size_t  CACHE_CAP = 0;        /* Allocated capacity of CACHE          */
static idx_t   CACHE_CNT = 0;        /* Number of terms stored so far        */
static size_t  NEXT_LEN  = 1;        /* Next number of digits to generate    */

/* Helper that generates all numbers of a given digit length
   and stores the primes into CACHE. */
static void generate_len(size_t len)
{
    /* Index array holding position in DIGITS for each digit:
       idx[0] is the leading digit (must be >=1).
       idx[i] for i>0 can be 0..4.                                  */
    size_t idx[32] = {0};
    /* Initialise leading digit to 1 (digit 3). */
    idx[0] = 1;
    /* Others start at 0 (digit 0). */

    for (size_t i = 1; i < len; ++i) idx[i] = 0;

    for (;;)
    {
        /* Build the number from the digit indices. */
        seq_t value = 0;
        for (size_t i = 0; i < len; ++i)
            value = value * 10 + DIGITS[idx[i]];

        /* Check for primality. */
        if (is_prime_big(value))
        {
            if (CACHE_CNT >= CACHE_CAP)
                grow_seq(&CACHE, &CACHE_CAP, CACHE_CNT + 1);
            CACHE[CACHE_CNT++] = value;
        }

        /* Increment the rightmost position. */
        size_t pos = len - 1;
        while (pos > 0 && (++idx[pos]) >= 5)
        {
            idx[pos] = 0;
            --pos;
        }
        if (pos == 0)        /* leading digit overflow */
        {
            if (++idx[0] >= 5)   /* finished all combinations of this length */
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* Undefined for offset 0, but harmless */

    /* Extend the cache until it contains at least n terms. */
    while (CACHE_CNT < n)
    {
        generate_len(NEXT_LEN);
        ++NEXT_LEN;
    }

    return CACHE[n - 1];
}
