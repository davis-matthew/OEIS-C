/*
 * A078290  Least nontrivial multiple of the n-th prime beginning with 6.
 * https://oeis.org/A078290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   Least nontrivial multiple of the n-th prime beginning with 6
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_len, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    /* Obtain the n-th prime. */
    uint32_t p = get_prime(n);

    /* Search for the smallest multiplier k ≥ 2 such that
       the decimal representation of p·k starts with 6. */
    for (seq_t k = 2; ; ++k) {
        seq_t prod = (seq_t)p * k;

        /* Number of decimal digits of prod. */
        int d = digit_len(prod);

        /* Compute 10^(d-1) to isolate the leading digit. */
        seq_t pow10 = 1;
        for (int i = 1; i < d; ++i) pow10 *= 10;

        /* First digit of prod. */
        int first = (int)((prod / pow10) % (seq_t)10);

        if (first == 6) return prod;
    }
}
