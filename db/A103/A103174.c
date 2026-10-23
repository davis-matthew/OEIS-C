/*
 * A103174  Numbers k with increasing digits such that the digits of k appear
 *          among the digits of the k-th prime number.
 * https://oeis.org/A103174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, full, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   digit_mask, get_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103174
   Numbers k with strictly increasing decimal digits such that each
   digit of k occurs among the digits of the k-th prime.
   The function returns the n‑th such number (offset 1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/digit_mask.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    /* static cache of the sequence */
    static seq_t *seq = NULL;      /* array of found k's */
    static size_t cap = 0;         /* allocated capacity (size_t as required by push_seq) */
    static size_t size = 0;        /* number of elements stored */
    static uint64_t next_k = 1;    /* next candidate k to test, fits into uint64_t because
                                     get_prime takes a uint64_t index */

    /* ignore undefined index 0 */
    if (n == 0) return 0;

    /* extend cache until it contains the n‑th term */
    while (size < n)
    {
        /* check that k has strictly increasing decimal digits */
        bool increasing = true;
        seq_t temp = (seq_t)next_k;
        int prev_digit = 10;              // larger than any decimal digit
        while (temp > 0)
        {
            int digit = (int)(temp % 10);
            if (digit >= prev_digit)
            {
                increasing = false;
                break;
            }
            prev_digit = digit;
            temp /= 10;
        }

        if (increasing)
        {
            seq_t prime_k = get_prime(next_k);
            uint16_t maskPrime = digit_mask(prime_k);
            uint16_t maskK     = digit_mask((seq_t)next_k);
            /* all digits of k must appear in the prime */
            if ( (maskK & ~maskPrime) == 0 )
            {
                push_seq(&seq, &cap, &size, (seq_t)next_k);
            }
        }
        ++next_k;
    }
    return seq[n - 1];             /* 0‑based indexing in the array */
}
