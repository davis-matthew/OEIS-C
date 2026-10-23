/*
 * A178447  a(n) = number of 4-digit primes with digit sum n, where n runs
 *          through the non-multiples of 3 in the range [2..34].
 * https://oeis.org/A178447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 4‑digit prime numbers whose digit sum equals
 * the nth non‑multiple of 3 in the interval [2..35] (the OEIS sequence
 * A178447).  The function builds a small sieve of primes once and then
 * looks up the required count.  It returns 0 for indices outside the
 * defined range. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* non‑multiples of 3 in [2..35] (23 values) */
static const idx_t non_mult3[] = {
    2, 4, 5, 7, 8,10,11,13,14,16,17,19,20,22,23,25,26,28,29,31,32,34,35
};
static const size_t non_mult3_len =
    sizeof(non_mult3) / sizeof(non_mult3[0]);

/* counts of 4‑digit primes by digit sum (indices 0..39) */
static seq_t digit_sum_counts[40];

/* flag indicating whether the table has been initialised */
static bool init_done = false;

/* Compute digit sum of a 4‑digit number (fits in idx_t). */
static idx_t digit_sum(uint16_t n)
{
    idx_t s = 0;
    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

/* Precompute digit‑sum counts for all 4‑digit primes.  This is done
 * only once, the first time the sequence function is called. */
static void init_sequence(void)
{
    if (init_done)
        return;

    /* clear array */
    memset(digit_sum_counts, 0, sizeof(digit_sum_counts));

    /* sieve up to 9999 */
    bool is_prime[10000];
    for (size_t i = 0; i < 10000; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint16_t i = 2; i * i < 10000; ++i) {
        if (is_prime[i]) {
            for (uint16_t j = i * i; j < 10000; j += i)
                is_prime[j] = false;
        }
    }

    /* count digit sums of 4‑digit primes */
    for (uint16_t n = 1000; n < 10000; ++n) {
        if (!is_prime[n])
            continue;
        idx_t s = digit_sum(n);
        if (s < 40)
            digit_sum_counts[s]++;
    }

    init_done = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    init_sequence();

    if (n == 0 || n > non_mult3_len)
        return (seq_t)0;

    idx_t sum = non_mult3[n - 1];
    if (sum >= 40)
        return (seq_t)0;
    return digit_sum_counts[sum];
}
