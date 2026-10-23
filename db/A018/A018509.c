/*
 * A018509  Divisors of 540.
 * https://oeis.org/A018509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate the positive divisors of 540 and return the
   nth divisor in ascending order (offset 1).  Divisors are
   generated once at first call and cached thereafter. */
#include <stddef.h>
#include <stdint.h>

enum { NUM_DIVS = 24 };                     /* 24 = (2+1)*(3+1)*(1+1) */

/* 540 = 2^2 * 3^3 * 5^1. */
static const uint64_t primes[3] = { 2, 3, 5 };
static const uint64_t exps[3]  = { 2, 3, 1 };

static seq_t divisors[NUM_DIVS];
static int   initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        size_t idx = 0;
        /* Enumerate all exponent combinations. */
        for (uint64_t e0 = 0; e0 <= exps[0]; ++e0) {
            seq_t val0 = 1;
            for (uint64_t i = 0; i < e0; ++i) val0 *= (seq_t)primes[0];
            for (uint64_t e1 = 0; e1 <= exps[1]; ++e1) {
                seq_t val1 = val0;
                for (uint64_t i = 0; i < e1; ++i) val1 *= (seq_t)primes[1];
                for (uint64_t e2 = 0; e2 <= exps[2]; ++e2) {
                    seq_t val = val1;
                    for (uint64_t i = 0; i < e2; ++i) val *= (seq_t)primes[2];
                    divisors[idx++] = val;
                }
            }
        }

        /* Simple insertion sort – the array has only 24 elements. */
        for (size_t i = 1; i < NUM_DIVS; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }

        initialized = 1;
    }

    if (n == 0 || n > NUM_DIVS) return 0;   /* out of range */
    return divisors[(size_t)(n - 1)];
}
