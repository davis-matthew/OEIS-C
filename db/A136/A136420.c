/*
 * A136420  List of primes with digits grouped into clumps of four. Leading
 *          zeros are not printed.
 * https://oeis.org/A136420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth 4‑digit block of the decimal concatenation of all primes
   in ascending order, with leading zeros suppressed. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* block construction */
    idx_t block_idx = 0;            /* number of completed 4‑digit blocks */
    int    digits_in_block = 0;     /* digits accumulated in current block */
    uint32_t block = 0;             /* numeric value of current block */

    /* dynamic array of known primes (used for trial division) */
    size_t prime_cap = 256;
    size_t prime_sz  = 0;
    uint32_t *primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) return 0;          /* allocation failed */

    uint64_t candidate = 2;         /* prime candidate, start from 2 */
    while (1) {
        /* check primality of candidate using known primes <= sqrt(candidate) */
        bool is_prime = true;
        for (size_t i = 0; i < prime_sz; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* store the new prime */
            if (prime_sz == prime_cap) {
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes,
                                                    prime_cap * sizeof(uint32_t));
                if (!tmp) { free(primes); return 0; }
                primes = tmp;
            }
            primes[prime_sz++] = (uint32_t)candidate;

            /* feed its decimal digits into the block builder */
            uint64_t tmp = candidate;
            uint8_t digits[20];
            int dlen = 0;
            do {
                digits[dlen++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            } while (tmp > 0);

            for (int i = dlen - 1; i >= 0; --i) {
                int d = digits[i];
                block = block * 10 + (uint32_t)d;
                ++digits_in_block;
                if (digits_in_block == 4) {
                    ++block_idx;
                    if (block_idx == n) {
                        free(primes);
                        return (seq_t)block;
                    }
                    block = 0;
                    digits_in_block = 0;
                }
            }
        }

        /* advance to next candidate */
        if (candidate == 2) candidate = 3;
        else                candidate += 2;
    }
}
