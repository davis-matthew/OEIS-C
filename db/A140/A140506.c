/*
 * A140506  Primes congruent to 11 or 19 mod 30.
 * https://oeis.org/A140506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A140506: primes congruent to 11 or 19 modulo 30.
 * Implementation: sieve of Eratosthenes up to a fixed limit,
 *  collect the primes, and return the n‑th element.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LIMIT 1000000   /* sufficient for n up to about 10 000 */

seq_t a(idx_t n)
{
    /* Static state for one‑time initialization */
    static bool *is_comp = NULL;
    static seq_t *seq     = NULL;
    static size_t seq_sz  = 0;      /* number of elements stored */
    static bool initialized = false;

    if (!initialized) {
        /* Allocate and initialise the composite array */
        is_comp = (bool *)malloc((MAX_LIMIT + 1) * sizeof(bool));
        if (!is_comp) return 0;
        for (idx_t i = 0; i <= MAX_LIMIT; ++i) is_comp[i] = false;
        is_comp[0] = true;
        is_comp[1] = true;

        /* Eratosthenes sieve */
        for (idx_t i = 2; i * i <= MAX_LIMIT; ++i) {
            if (!is_comp[i]) {
                for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }

        /* Prepare output array */
        size_t capacity = MAX_LIMIT / 4 + 10;   /* very safe estimate */
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!seq) {
            free(is_comp);
            return 0;
        }

        /* Collect primes ≡ 11 or 19 (mod 30) in increasing order */
        seq_sz = 0;
        for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
            if (!is_comp[i] && ((i % 30) == 11 || (i % 30) == 19)) {
                if (seq_sz == capacity) {
                    capacity *= 2;
                    seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
                    if (!seq) { free(is_comp); return 0; }
                }
                seq[seq_sz++] = (seq_t)i;
            }
        }

        initialized = true;
    }

    if (n < 1 || (idx_t)n > seq_sz) return 0;
    return seq[n - 1];
}
