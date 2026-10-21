/*
 * A050668  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 6.
 * https://oeis.org/A050668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A050668 by generating primes in ascending order
   and counting the number of distinct single‑digit variations that are
   also prime. A prime enters the sequence if exactly six such variations
   exist. The function maintains a static cache of the sequence values
   so that repeated calls require no recomputation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;          /* cache of sequence values      */
    static size_t seq_len = 0;         /* number of values cached       */
    static size_t seq_cap = 0;         /* allocated capacity            */
    static idx_t last_candidate = 2;   /* next integer to test          */

    /* ensure the cache is allocated */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
    }

    /* Generate sequence values until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find next prime candidate */
        seq_t cand = (seq_t)last_candidate;
        last_candidate++;               /* next number to check            */

        /* skip non‑primes */
        if (!is_prime_big(cand))
            continue;

        /* count the number of prime neighbours obtained by changing
           exactly one decimal digit, keeping the same digit length   */
        int len = digit_len(cand);
        int count = 0;

        for (int pos = 0; pos < len; ++pos) {
            int exp = len - 1 - pos;                /* 10^exp */
            seq_t pow = pow10((idx_t)exp);
            seq_t q = cand / pow;
            int orig_digit = (int)(q % 10);      /* digit at position pos */

            for (int d = 0; d <= 9; ++d) {
                if (d == orig_digit) continue;
                /* disallow leading zero */
                if (pos == 0 && d == 0) continue;

                /* compute new number by increasing or decreasing that digit */
                int diff = d - orig_digit;
                seq_t new_q;
                if (diff > 0)
                    new_q = cand + (seq_t)diff * pow;
                else
                    new_q = cand - (seq_t)(-diff) * pow;

                if (is_prime_big(new_q))
                    ++count;
            }
        }

        /* if exactly six variations are prime, store the prime */
        if (count == 6) {
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap * 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) exit(EXIT_FAILURE);
            }
            seq[seq_len++] = cand;
        }
    }

    return seq[n - 1];
}
