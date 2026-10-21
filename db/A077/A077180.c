/*
 * A077180  Smallest concatenation of integers starting from 1 that is a
 *          multiple of the n-th prime.
 * https://oeis.org/A077180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   digit_count, get_prime, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: smallest integer formed by concatenating 1,2,...,m such that
   it is a multiple of the n‑th prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../utils/get_prime.h"     /* idx_t get_prime(idx_t k); */
#include "../utils/digit_count.h"   /* uint64_t digit_count(uint64_t n); */
#include "../utils/seq_from_str.h"  /* seq_t seq_from_str(const char *s); */

/* Compute a(n) for OEIS sequence A077180. */
seq_t a(idx_t n)
{
    /* Undefined for offset 1 but harmless. */
    if (n == 0)
        return 0;

    /* The required prime.  The helper returns it as idx_t but
       we know it fits in 64 bits. */
    uint64_t prime = (uint64_t)get_prime(n);

    /* Working modulo the prime. */
    uint64_t mod = 0;

    /* Temporary buffer to build the decimal representation. */
    size_t buf_cap = 1024;
    size_t buf_len = 0;
    char *buf = (char *)malloc(buf_cap);
    if (!buf)
        return 0;   /* allocation failure */

    /* Loop over consecutive integers to find the first divisible
       concatenation. */
    for (uint64_t k = 1; ; ++k)
    {
        /* Number of decimal digits of k. */
        uint64_t digits = digit_count(k);

        /* Compute 10^digits mod prime. */
        uint64_t pow10mod = 1 % prime;
        for (uint64_t i = 0; i < digits; ++i)
            pow10mod = (pow10mod * 10ULL) % prime;

        /* Update the modulo value for the new concatenation. */
        mod = (mod * pow10mod + k) % prime;

        /* Append k to the buffer as decimal text. */
        int written = snprintf(buf + buf_len, buf_cap - buf_len,
                               "%llu", (unsigned long long)k);
        if (written < 0) {
            free(buf);
            return 0;   /* encoding error */
        }
        buf_len += (size_t)written;

        /* Enlarge the buffer if its remaining space is too small. */
        if (buf_cap - buf_len < 64) {
            size_t new_cap = buf_cap * 2;
            char *tmp = (char *)realloc(buf, new_cap);
            if (!tmp) {
                free(buf);
                return 0;   /* reallocation failure */
            }
            buf = tmp;
            buf_cap = new_cap;
        }

        /* Stop once we hit a concatenation divisible by the prime. */
        if (mod == 0)
            break;
    }

    /* Convert the final concatenated decimal string to seq_t. */
    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
