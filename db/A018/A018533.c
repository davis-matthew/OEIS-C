/*
 * A018533  Divisors of 585.
 * https://oeis.org/A018533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 585: a(n) is the nth divisor of 585 in ascending order. */
#include <stddef.h>
#include <stdbool.h>

static const uint8_t primes[3]   = {3, 5, 13};
static const uint8_t exps[3]     = {2, 1, 1};

static bool initialized = false;
static seq_t divisors[12];

static void generate_divisors(void)
{
    size_t idx = 0;
    for (uint8_t a = 0; a <= exps[0]; ++a) {
        seq_t t0 = 1;
        for (uint8_t i = 0; i < a; ++i) t0 *= (seq_t)primes[0];
        for (uint8_t b = 0; b <= exps[1]; ++b) {
            seq_t t1 = t0;
            for (uint8_t i = 0; i < b; ++i) t1 *= (seq_t)primes[1];
            for (uint8_t c = 0; c <= exps[2]; ++c) {
                seq_t t2 = t1;
                for (uint8_t i = 0; i < c; ++i) t2 *= (seq_t)primes[2];
                divisors[idx++] = t2;
            }
        }
    }
    /* insertion sort – negligible cost for 12 elements */
    for (size_t i = 1; i < idx; ++i) {
        seq_t key = divisors[i];
        int j = (int)i - 1;
        while (j >= 0 && divisors[j] > key) {
            divisors[j + 1] = divisors[j];
            --j;
        }
        divisors[j + 1] = key;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1 */
    if (!initialized) generate_divisors(); /* lazy initialization */
    if (n <= 12) return divisors[(size_t)n - 1];
    return (seq_t)0;                       /* sequence has only 12 terms */
}
