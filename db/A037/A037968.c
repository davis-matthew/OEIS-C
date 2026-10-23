/*
 * A037968  Greatest number of distinct digits of n in any base b>=2.
 * https://oeis.org/A037968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = greatest number of distinct digits of n in any base b ≥ 2 */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case for n = 1 */
    if (n < 2) {
        return (seq_t)1;
    }

    seq_t best = (seq_t)1;   /* at least one digit in base 2 */
    /* A safe upper bound for the number of digits of a 64‑bit integer in any base */
    seq_t digits[128];

    for (idx_t base = 2; base <= n; ++base) {
        seq_t tmp = (seq_t)n;
        seq_t base_seq = (seq_t)base;
        size_t distinct = 0;

        while (tmp > 0) {
            seq_t digit = tmp % base_seq;

            bool unique = true;
            for (size_t i = 0; i < distinct; ++i) {
                if (digits[i] == digit) {
                    unique = false;
                    break;
                }
            }

            if (unique) {
                digits[distinct++] = digit;
            }

            tmp /= base_seq;
        }

        seq_t distinct_seq = (seq_t)distinct;
        if (distinct_seq > best) {
            best = distinct_seq;
        }
    }

    return best;
}
