/*
 * A156299  Primes where the first digit is greater than any other digit.
 * https://oeis.org/A156299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose first digit is strictly greater than
 *            any other digit
 * Range:     u8 n <= 10, u16 n <= 287, u32 n <= 369, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static bool is_valid(seq_t p) {
    /* A prime must have at least two digits and first digit > all other digits. */
    if (p < 10) return false;

    /* Extract most significant digit. */
    seq_t msd = p;
    while (msd >= 10)
        msd /= 10;

    /* Scan all digits except the most significant one. */
    seq_t tmp = p;
    while (tmp > 0) {
        if (tmp == msd) {          /* reached the most significant digit – skip it */
            tmp /= 10;
            continue;
        }
        seq_t d = tmp % 10;
        if (d >= msd)             /* any other digit not smaller */
            return false;
        tmp /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *seq_arr = NULL;   /* cached sequence */
    static size_t seq_len = 0;      /* number of elements in cache */
    static size_t seq_cap = 0;      /* capacity of cache */
    static seq_t current = 2;       /* candidate prime for next check */

    while (seq_len < n) {
        if (is_prime_big(current) && is_valid(current)) {
            append_candidate(&seq_arr, &seq_len, &seq_cap, current);
        }
        current += 1;
    }
    return seq_arr[n - 1];
}
