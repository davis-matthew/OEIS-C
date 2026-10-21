/*
 * A065138  Numbers that are equal to the sum of their digits plus the sum of
 *          cubes of their digits.
 * https://oeis.org/A065138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = numbers equal to the sum of their digits plus * the sum
 *            of the cubes of their digits (OEIS A065138)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static bool prepared = false;

static void prepare_terms(void) {
    if (prepared) return;
    prepared = true;

    /* The largest possible value is when all digits are 9:
     * sum_of_digits <= 9 * d,
     * sum_of_cubes <= 9^3 * d = 729 * d.
     * For d = 5, the maximum sum is 738 * 5 = 3690.
     * Numbers with 6 digits are > 100000 > 738 * 6, so no solutions exist
     * beyond 5 digits.  We scan all values up to 5000 to be safe. */
    const seq_t limit = 5000;
    terms_cap = 64;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) return;  /* out of memory – sequence will be empty */

    for (seq_t x = 0; x <= limit; ++x) {
        seq_t sum_digits = 0;
        seq_t sum_cubes = 0;
        seq_t y = x;
        if (y == 0) {
            /* 0 has digit sum 0 and cube sum 0 */
            sum_digits = 0;
            sum_cubes = 0;
        } else {
            while (y > 0) {
                unsigned d = (unsigned)(y % 10);
                sum_digits += d;
                sum_cubes += (seq_t)d * d * d;
                y /= 10;
            }
        }
        if (x == sum_digits + sum_cubes) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) return;
            }
            terms[terms_len++] = x;
        }
    }
}

extern seq_t a(idx_t n) {
    prepare_terms();
    if (n == 0 || n > terms_len) return 0;
    return terms[n - 1];
}
