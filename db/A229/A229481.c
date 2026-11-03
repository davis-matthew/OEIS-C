/*
 * A229481  Final digit of 1^n + 2^n + ... + n^n.
 * https://oeis.org/A229481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = final digit of ( 1^n + 2^n + … + n^n ). Period = 100
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 116 terms through n = 1000 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute base^exp (mod 10).  base may be up to 99, exp up to 99. */
static int mod10_pow(int base, int exp)
{
    int result = 1;
    base %= 10;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % 10;
        base = (base * base) % 10;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    static seq_t table[100];
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 0; i < 100; ++i) {
            int sum = 0;
            if (i == 0) {
                sum = 0;
            } else {
                for (idx_t k = 1; k <= i; ++k) {
                    sum += mod10_pow((int)k, (int)i);
                }
                sum %= 10;
            }
            table[i] = (seq_t)sum;
        }
        initialized = true;
    }

    idx_t idx = n % 100;
    return table[idx];
}
