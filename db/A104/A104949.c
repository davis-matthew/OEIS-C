/*
 * A104949  Primes from merging of 6 successive digits in decimal expansion of
 *          the Champernowne Constant.
 * https://oeis.org/A104949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n‑th prime that appears as a 6‑digit substring with no
 *            leading zero in the decimal expansion of the Champernowne
 *            constant (the concatenation 1,2,3,4,5,…)
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Global state retained across calls to a(). */
static seq_t *prime_buffer = NULL;  /* array of primes found so far   */
static size_t prime_cap   = 0;      /* current allocation size       */
static size_t prime_cnt   = 0;      /* number of primes stored       */

static uint64_t next_int     = 1;   /* next integer to output in stream */
static uint8_t digit_buf[20];      /* MSB‑first digits of current int   */
static int     digit_len      = 0;  /* number of digits in digit_buf      */
static int     digit_pos      = 0;  /* next position in digit_buf         */

static uint32_t window_value = 0;   /* value of the current 6‑digit window */
static int     window_len   = 0;    /* number of digits seen in window    */

/* Return the next decimal digit of Champernowne's constant. */
static uint8_t next_cham_digit(void)
{
    if (digit_pos == digit_len) {           /* need a new integer */
        uint64_t t = next_int++;
        int idx = 0;
        if (t == 0) {
            digit_buf[0] = 0;
            idx = 1;
        } else {
            while (t > 0) {                 /* extract digits LSB‑first */
                digit_buf[idx++] = (uint8_t)(t % 10);
                t /= 10;
            }
            /* reverse to most‑significant‑first order */
            for (int i = 0; i < idx/2; ++i) {
                uint8_t tmp = digit_buf[i];
                digit_buf[i] = digit_buf[idx-1-i];
                digit_buf[idx-1-i] = tmp;
            }
        }
        digit_len = idx;
        digit_pos = 0;
    }
    return digit_buf[digit_pos++];
}

/* Grow the prime buffer when needed. */
static void grow_prime_buffer(void)
{
    size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
    seq_t *tmp = (seq_t *)realloc(prime_buffer, new_cap * sizeof(seq_t));
    if (tmp == NULL) exit(1);                 /* memory exhausted */
    prime_buffer = tmp;
    prime_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined, for safety */

    /* Make sure the buffer can hold at least n primes. */
    while (prime_cap < n) grow_prime_buffer();

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < (size_t)n) {
        uint8_t d = next_cham_digit();

        /* Update the sliding 6‑digit window. */
        if (window_len < 6) {
            window_value = window_value * 10 + d;
            ++window_len;
        } else {
            window_value = (window_value % 100000) * 10 + d;   /* drop MSB */
        }

        /* Test the window when it has 6 digits and no leading zero. */
        if (window_len == 6 && window_value >= 100000) {
            if (is_prime((uint64_t)window_value)) {
                prime_buffer[prime_cnt++] = (seq_t)window_value;
            }
        }
    }

    return prime_buffer[n-1];
}
