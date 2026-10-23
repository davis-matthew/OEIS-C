/*
 * A008568  Digits of powers of 7.
 * https://oeis.org/A008568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4317, u16 n <= 4317, u32 n <= 4317, u64 n <= 4317,
 *            u128 n <= 4317, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008568: a(n) is the n‑th digit in the table obtained by writing successive
   powers of 7 in decimal, row by row. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static int power_count = 0;
static uint8_t *powers[1024];         /* digits of 7^k, most‑significant first */
static size_t power_sizes[1024];      /* number of digits of each 7^k       */
static uint64_t prefix[1025];         /* cumulative digits, prefix[i] = sum_{k< i} power_sizes[k] */

/* initialise the cache with 7^0 = 1 */
static void init_cache(void) {
    power_count = 1;
    uint8_t *d0 = (uint8_t*)malloc(1);
    if (!d0) exit(1);
    d0[0] = 1;
    powers[0] = d0;
    power_sizes[0] = 1;
    prefix[0] = 0;
    prefix[1] = 1;
    initialized = true;
}

/* grow the cache until the index n is covered */
static void ensure_cache(idx_t n) {
    if (!initialized) init_cache();
    while (prefix[power_count] <= n) {
        /* previous power */
        const uint8_t *prev = powers[power_count - 1];
        size_t prev_len = power_sizes[power_count - 1];

        /* multiply by 7, building digits least‑significant first */
        uint8_t *tmp = (uint8_t*)malloc(prev_len + 1);
        if (!tmp) exit(1);
        size_t idx = 0;
        int carry = 0;
        for (int i = (int)prev_len - 1; i >= 0; --i) {
            int prod = prev[i] * 7 + carry;
            tmp[idx++] = prod % 10;
            carry = prod / 10;
        }
        while (carry > 0) {
            tmp[idx++] = carry % 10;
            carry /= 10;
        }

        /* reverse to get most‑significant first */
        uint8_t *new_digits = (uint8_t*)malloc(idx);
        if (!new_digits) exit(1);
        for (size_t j = 0; j < idx; ++j)
            new_digits[j] = tmp[idx - 1 - j];
        free(tmp);

        /* store the new power */
        power_sizes[power_count] = idx;
        powers[power_count] = new_digits;

        /* update cumulative prefix */
        prefix[power_count + 1] = prefix[power_count] + idx;
        power_count++;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    /* locate the row that contains index n */
    int row = 0;
    while (prefix[row + 1] <= n) ++row;
    idx_t pos = n - prefix[row];
    return (seq_t)powers[row][pos];
}
