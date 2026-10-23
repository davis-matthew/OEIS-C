/*
 * A144753  Positive integers whose binary representation is a palindrome and
 *          has a prime number of 1's.
 * https://oeis.org/A144753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 106, u32 n <= 9252, u64 n <= 9252,
 *            u128 n <= 9252, big any n
 * Helpers:   init_prime_table
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144753
   Positive integers whose binary representation is a palindrome
   and has a prime number of 1's.
   The function searches binary palindromes in increasing order and
   returns the n‑th term that satisfies that property.
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/init_prime_table.h"

/* build a table of prime flags up to PRIME_TABLE_SIZE-1.
   The table is only needed for pop‑count values, which never exceed
   about 128 for the sizes encountered here. */
#define PRIME_TABLE_SIZE 128

/* Main sequence generation function */
seq_t a(idx_t n) {
    /* Ensure the prime table is built only once */
    static bool prime_table[PRIME_TABLE_SIZE] = { 0 };
    static bool table_ready = false;
    if (!table_ready) {
        init_prime_table(prime_table, PRIME_TABLE_SIZE);
        table_ready = true;
    }

    /* Search for the n‑th palindrome with a prime pop‑count */
    idx_t found = 0;                      /* count of valid numbers found */
    for (idx_t len = 1;; ++len) {         /* length of the binary palindrome */
        /* For lengths 1 and 2 the leading bit is always 1; we skip
           length 1 because pop‑count=1 is not prime. */
        if (len == 1) continue;

        idx_t half_len  = (len + 1) / 2;   /* bits in the first half (incl. middle) */
        /* Prevent shift overflow when half_len >= 64.  For the values
           of n we handle (up to ~10000) this never happens. */
        if (half_len - 1 >= 63) break;

        idx_t half_start = ((idx_t)1 << (half_len - 1));   /* leading bit = 1 */
        idx_t half_end   = ((idx_t)1 << half_len)   - 1;

        for (idx_t half = half_start; half <= half_end; ++half) {
            /* Count number of 1's in the first half */
            idx_t tmp_half = half;
            int  ones = 0;
            while (tmp_half) {
                ones += tmp_half & 1;
                tmp_half >>= 1;
            }

            /* Build the full palindrome and count additional 1's */
            seq_t pal  = (seq_t)half;          /* start with the first half */
            idx_t tmp  = (len & 1) ? (half >> 1) : half;  /* mirror part */
            while (tmp) {
                pal = (pal << 1) | (tmp & 1);
                if (tmp & 1) ++ones;
                tmp >>= 1;
            }

            /* Accept if pop‑count is prime */
            if (ones < PRIME_TABLE_SIZE && prime_table[ones]) {
                ++found;
                if (found == n) return pal;
            }
        }
    }
    /* Should never be reached */
    return 0;
}
