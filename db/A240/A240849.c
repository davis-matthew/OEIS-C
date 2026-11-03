/*
 * A240849  Quinary happy numbers.
 * https://oeis.org/A240849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   quinary happy numbers – computed by testing each integer
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9499 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sum of squares of the base‑5 digits of v. */
static seq_t next_base5_square_sum(seq_t v)
{
    seq_t sum = (seq_t)0;
    seq_t x = v;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)5;      /* digit in base 5 */
        sum += d * d;                /* add its square */
        x /= (seq_t)5;               /* move to next digit */
    }
    return sum;
}

/* Test whether v is a quinary happy number. */
static bool is_quinary_happy(seq_t v)
{
    if (v == (seq_t)1) return true;

    seq_t tortoise = next_base5_square_sum(v);
    seq_t hare     = next_base5_square_sum(next_base5_square_sum(v));

    if (tortoise == (seq_t)1 || hare == (seq_t)1)
        return true;

    /* Floyd's cycle‑finding algorithm. */
    while (tortoise != hare) {
        tortoise = next_base5_square_sum(tortoise);
        hare     = next_base5_square_sum(next_base5_square_sum(hare));
        if (tortoise == (seq_t)1 || hare == (seq_t)1)
            return true;
    }
    return false;   /* cycle detected that does not contain 1 */
}

/* a(n) returns the n-th quinary happy number (offset 1). */
seq_t a(idx_t n)
{
    idx_t count = (idx_t)0;
    seq_t candidate = (seq_t)1;

    while (true) {
        if (is_quinary_happy(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}
