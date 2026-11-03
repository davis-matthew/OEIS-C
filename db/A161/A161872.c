/*
 * A161872  Smallest unhappy number in base n (or 0 if no unhappy numbers in
 *          the base).
 * https://oeis.org/A161872
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest unhappy number in base n (or 0 if none)
 * Range:     u8 n <= 17, u16 n <= 10081, u32 n <= 10082, u64 n <= 10082,
 *            u128 n <= 10082, big any n
 * Validated: standard, pass, 115 terms through n = 8760 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Sum of squares of the digits of x in the given base */
static inline seq_t sum_sq_digits(seq_t x, seq_t base)
{
    seq_t sum = 0;
    while (x != 0) {
        seq_t d = x % base;
        sum += d * d;
        x /= base;
    }
    return sum;
}

/* Determine whether m is a happy number in the given base. */
static inline bool is_happy(seq_t base, seq_t m)
{
    if (m == 1) return true;
    seq_t tortoise = sum_sq_digits(m, base);
    seq_t hare = sum_sq_digits(tortoise, base);
    while (tortoise != hare) {
        tortoise = sum_sq_digits(tortoise, base);
        hare = sum_sq_digits(hare, base);
        hare = sum_sq_digits(hare, base);   /* advance hare twice */
    }
    return tortoise == 1;
}

/* OEIS sequence A161872: smallest unhappy number in base n (or 0 if none) */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* offset is 2; value undefined below it */

    seq_t base = (seq_t)n;

    /* Bases 2 and 4 are known to have no unhappy numbers */
    if (base == 2 || base == 4) return 0;

    seq_t m = 2;
    while (true) {
        if (!is_happy(base, m))
            return m;                    /* smallest unhappy number found */
        ++m;
    }
}
