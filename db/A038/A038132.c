/*
 * A038132  Third earliest sequence of 6 primes according to the rules
 *          stipulated in A005150.
 * https://oeis.org/A038132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A038132.
   The sequence is defined by repeated look‑and‑say expansions
   starting from the seed number 402266411.
   Term a(1) is the seed itself, and for n > 1:
        a(n) = look_and_say(a(n-1)),
   where look_and_say reads each run of equal digits and
   outputs the count followed by the digit. */

#include <stdint.h>
#include <stdlib.h>

/* Helper: convert a decimal integer value (≤ UINT64_MAX) into an
   array of its decimal digits, most significant first. */
static void digits_of_uint64(uint64_t value, uint8_t **buf, size_t *len)
{
    /* Count digits */
    size_t d = 0;
    uint64_t tmp = value;
    do {
        d++;
        tmp /= 10U;
    } while (tmp > 0U);

    *buf = (uint8_t*)malloc(d * sizeof(uint8_t));
    *len = d;
    tmp = value;
    for (size_t i = 0; i < d; ++i) {
        (*buf)[d - 1 - i] = (uint8_t)(tmp % 10U);
        tmp /= 10U;
    }
}

seq_t a(idx_t n)
{
    /* Seed value fits in a 32‑bit unsigned integer */
    const uint64_t seed = 402266411ULL;

    /* Handle the first term quickly */
    if (n == 1) {
        return (seq_t)seed;
    }

    /* Current digits array (most significant first) */
    uint8_t *cur = NULL;
    size_t cur_len = 0;
    digits_of_uint64(seed, &cur, &cur_len);

    /* Perform (n-1) look‑and‑say steps */
    for (idx_t step = 2; step <= n; ++step) {
        /* Allocate a new buffer: next length never exceeds
           cur_len * 5 + 10 (safe upper bound). */
        size_t next_cap = cur_len * 5 + 10;
        uint8_t *next = (uint8_t*)malloc(next_cap * sizeof(uint8_t));
        size_t next_len = 0;

        size_t i = 0;
        while (i < cur_len) {
            uint8_t digit = cur[i];
            size_t j = i + 1;
            while (j < cur_len && cur[j] == digit) {
                j++;
            }
            /* Run length */
            size_t run = j - i;

            /* Convert run length to decimal digits */
            uint8_t digit_buf[32];
            size_t dlen = 0;
            size_t c = run;
            do {
                digit_buf[dlen++] = (uint8_t)(c % 10U);
                c /= 10U;
            } while (c > 0U);

            /* Append run length digits in correct order */
            for (size_t k = dlen; k > 0; --k) {
                next[next_len++] = digit_buf[k - 1];
            }
            /* Append the digit itself */
            next[next_len++] = digit;

            i = j;
        }

        /* Release previous buffer and move to the next iteration */
        free(cur);
        cur = next;
        cur_len = next_len;
    }

    /* Convert the final digit array back to a seq_t value */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cur_len; ++i) {
        result = result * ((seq_t)10) + ((seq_t)cur[i]);
    }

    free(cur);
    return result;
}
