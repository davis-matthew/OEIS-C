/*
 * A161871  Smallest happy number > 1 in base n.
 * https://oeis.org/A161871
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 707, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest happy number > 1 in base n (search).  */
#include <stdbool.h>

/* Sum of the squares of the digits of x in the given base.  */
static seq_t sumsq_digits(seq_t x, seq_t base)
{
    seq_t sum = 0;
    while (x != 0) {
        seq_t d = x % base;
        sum += d * d;
        x   /= base;
    }
    return sum;
}

/* Test whether x is happy in the given base.  */
static bool is_happy(seq_t x, seq_t base)
{
    seq_t slow = x;
    seq_t fast = x;

    while (true) {
        slow = sumsq_digits(slow, base);          /* step 1 */
        if (slow == 1) return true;               /* reached 1 */

        fast = sumsq_digits(fast, base);          /* first step for fast */
        fast = sumsq_digits(fast, base);          /* second step for fast */
        if (fast == 1) return true;               /* reached 1 */

        if (slow == fast) return false;           /* loop detected */
        /* continue with next iteration; slow and fast already updated   */
    }
}

/* OEIS A161871: smallest happy number > 1 in base n.  */
seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n < 2, not used */

    seq_t base = (seq_t)n;
    seq_t candidate = 2;

    while (true) {
        if (is_happy(candidate, base))
            return candidate;
        ++candidate;
    }
}
