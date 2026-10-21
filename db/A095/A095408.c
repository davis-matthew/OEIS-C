/*
 * A095408  Total number of decimal digits in all distinct prime factors of n
 *          minus number of digits in n.
 * https://oeis.org/A095408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = (sum of decimal digits of all distinct prime factors of
 *            n) - (decimal digits of n)
 * Range:     i8 n <= 9436, i16 n <= 9436, i32 n <= 9436, i64 n <= 9436,
 *            i128 n <= 9436, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper to count decimal digits of a non‑negative integer (uint64_t). */
static int num_digits_uint64(uint64_t v)
{
    int cnt = 1;
    while (v >= 10) {
        v /= 10;
        cnt++;
    }
    return cnt;
}

/* a(n) as defined in OEIS A095408. */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    int digits_n = num_digits_uint64(nn);

    /* Sum digits of distinct prime divisors. */
    seq_t digit_sum = 0;

    /* Handle factor 2 separately. */
    if (nn % 2 == 0) {
        digit_sum += (seq_t)num_digits_uint64(2);
        while (nn % 2 == 0) nn /= 2;
    }

    /* Odd factors. */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p == 0) {
            digit_sum += (seq_t)num_digits_uint64(p);
            while (nn % p == 0) nn /= p;
        }
    }

    /* Last prime factor > sqrt(original n). */
    if (nn > 1) {
        digit_sum += (seq_t)num_digits_uint64(nn);
    }

    /* Result: sum of digits of distinct primes minus digits of n. */
    return digit_sum - (seq_t)digits_n;
}
