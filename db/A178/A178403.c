/*
 * A178403  Numbers containing the rounded up arithmetic mean of their digits
 *          at least once, cf. A004427.
 * https://oeis.org/A178403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9736 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers containing the rounded up arithmetic mean of their digits
   at least once (OEIS A178403).  A(n) is the n‑th such integer
   (a(1) = 0). */

#include <stdbool.h>
#include <stddef.h>

/* Helper: test whether a number contains the rounded‑up mean of its
   digits at least once. */
static bool contains_mean_ceil(seq_t x)
{
    /* Special case 0 */
    if (x == 0) return true;

    seq_t sum = 0;
    seq_t cnt = 0;
    bool seen[10] = {false};

    seq_t temp = x;
    while (temp > 0) {
        seq_t digit = temp % 10;
        sum += digit;
        cnt += 1;
        seen[(int)digit] = true;
        temp /= 10;
    }

    /* ceil(mean) = (sum + cnt - 1) / cnt */
    seq_t mean_ceil = (sum + cnt - 1) / cnt;
    return seen[(int)mean_ceil];
}

/* Generates the sequence a(n) = A178403. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t num = 0;

    while (true) {
        if (contains_mean_ceil(num)) {
            found++;
            if (found == n) return num;
        }
        num += 1;
    }
}
