/*
 * A158699  Start with 0; then add one to each single digit.
 * https://oeis.org/A158699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = fⁿ(0), where f adds 1 to every decimal digit of x,
 *            replacing 9 by 0 unless it is the left‑most digit, which then
 *            becomes 10 (digits “1” then “0”)
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 84, u64 n <= 172,
 *            u128 n <= 345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t next_term(seq_t x) {
    /* Special case: 0 → 1 */
    if (x == (seq_t)0) return (seq_t)1;

    /* Find the highest power of 10 <= x */
    seq_t pow10 = (seq_t)1;
    while (x / pow10 >= (seq_t)10) {
        pow10 *= (seq_t)10;
    }

    seq_t result = (seq_t)0;
    bool first = true;

    /* Process digits from most significant to least */
    for (; pow10 > (seq_t)0; pow10 /= (seq_t)10) {
        seq_t d = (x / pow10) % (seq_t)10;

        if (first) {
            if (d == (seq_t)9) {
                /* Leading 9: produce “10” */
                result = result * (seq_t)10 + (seq_t)1;
                result = result * (seq_t)10 + (seq_t)0;
            } else {
                result = result * (seq_t)10 + (d + (seq_t)1);
            }
            first = false;
        } else {
            seq_t new_digit = (d == (seq_t)9) ? (seq_t)0 : (d + (seq_t)1);
            result = result * (seq_t)10 + new_digit;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    /* Iterate the transformation n times starting from 0 */
    seq_t cur = (seq_t)0;
    for (idx_t i = (idx_t)0; i < n; ++i) {
        cur = next_term(cur);
    }
    return cur;
}
