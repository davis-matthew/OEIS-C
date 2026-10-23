/*
 * A190479  Let s the sum of the decimal digit of a number k. Number of
 *          integer solutions of the equation s*(s+n)- k = 0.
 * https://oeis.org/A190479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2967, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A190479 – Count the solutions of s*(s+n) = k with digit_sum(k) = s.
 * a(n) = |{ s ∈ ℕ⁺ : digit_sum( s*(s+n) ) == s }|,
 * for n ≥ 1 (offset 1).  The implementation tests successive values
 * of s until the maximum possible digit sum of k is exceeded.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute decimal digit sum of x.  Works for all supported seq_t
 * types (built‑in unsigned integers or arbitrary‑precision files).
 */
static seq_t digit_sum_local(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x   /= 10;
    }
    return sum;
}

/* Compute number of decimal digits of x.  Returns an idx_t
 * (safe for small values; digit count never exceeds 3 for uint8_t,
 * 4 for uint16_t, etc.).
 */
static idx_t digit_len_local(seq_t x)
{
    idx_t len = 0;
    do {
        ++len;
        x /= 10;
    } while (x > 0);
    return len;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t nval  = (seq_t)n;          /* cast once for the loop */

    /* The OEIS offset is 1; for safety return 0 for n==0. */
    if (nval == 0) {
        return 0;
    }

    for (seq_t s = 1; ; ++s) {
        seq_t k = s * (s + nval);   /* k = s*(s+n) */

        if (digit_sum_local(k) == s) {
            ++count;
        }

        /* No further s can satisfy the condition once s exceeds the
         * maximum possible digit sum of k.  The max digit sum is 9
         * times the number of decimal digits of k.
         */
        seq_t max_digit_sum = 9 * (seq_t)digit_len_local(k);
        if (s > max_digit_sum) {
            break;
        }
    }

    return count;
}
