/*
 * A099646  Function f(n) = 1 + Sum(digit^2 of n) is iterated and a(n) is the
 *          length of terminal cycle at initial value n.
 * https://oeis.org/A099646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the terminal cycle obtained by iterating f(x) =
 *            1 + sum of the squares of the decimal digits of x starting from
 *            n
 * Range:     u8 n <= 5950, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the sum of squares of decimal digits of x. */
static seq_t digit_square_sum(seq_t x) {
    seq_t sum = 0;
    const seq_t ten = (seq_t)10;
    if (x == 0) return 0;
    while (x != 0) {
        seq_t d = x % ten;
        sum += d * d;
        x /= ten;
    }
    return sum;
}

/* One application of f(x). */
static seq_t f_next(seq_t x) {
    return (seq_t)1 + digit_square_sum(x);
}

/* Compute a(n) using Floyd’s cycle–finding algorithm. */
seq_t a(idx_t n) {
    seq_t start = (seq_t)n;

    /* Find first meeting point of tortoise and hare. */
    seq_t tortoise = f_next(start);
    seq_t hare = f_next(f_next(start));
    while (tortoise != hare) {
        tortoise = f_next(tortoise);
        hare = f_next(f_next(hare));
    }

    /* Find the cycle length. */
    seq_t cycle_start = tortoise;
    seq_t length = 1;
    hare = f_next(cycle_start);
    while (hare != cycle_start) {
        hare = f_next(hare);
        ++length;
    }
    return length;
}
