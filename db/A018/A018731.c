/*
 * A018731  Divisors of 936.
 * https://oeis.org/A018731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 936, sorted ascending */
#include <stdlib.h>
#include <stdbool.h>

/* static cache for the 24 divisors of 936 */
static seq_t divisors[24];
static size_t divisor_count = 0;
static bool initialized = false;

/* initialise the cache (executed once) */
static void init_divisors(void)
{
    if (initialized) {
        return;
    }
    initialized = true;

    /* prime factorisation of 936 = 2^3 * 3^2 * 13^1 */
    const int primes[] = {2, 3, 13};
    const int exps[]  = {3, 2, 1};
    const size_t np = sizeof primes / sizeof primes[0];

    size_t pos = 0;
    divisors[pos++] = (seq_t)1;          /* start with divisor 1 */

    /* generate all divisors multiplicatively */
    for (size_t i = 0; i < np; ++i) {
        int p   = primes[i];
        int e   = exps[i];
        size_t old = pos;                 /* count before adding this factor */

        for (size_t j = 0; j < old; ++j) {
            unsigned long long base = (unsigned long long)divisors[j];
            unsigned long long cur  = base;

            for (int k = 1; k <= e; ++k) {
                cur *= (unsigned long long)p;
                divisors[pos++] = (seq_t)cur;
            }
        }
    }

    divisor_count = pos;

    /* sort the divisors in ascending order (insertion sort, 24 items) */
    for (size_t i = 1; i < divisor_count; ++i) {
        seq_t key = divisors[i];
        int j = (int)i - 1;
        while (j >= 0 && divisors[j] > key) {
            divisors[j + 1] = divisors[j];
            --j;
        }
        divisors[j + 1] = key;
    }
}

/* OEIS sequence A018731: a(n) = n-th divisor of 936 (ignoring n=0) */
seq_t a(idx_t n)
{
    init_divisors();

    if (n < 1 || n > (idx_t)divisor_count) {
        return (seq_t)0;   /* out of range; the sequence is finite */
    }

    return divisors[(size_t)(n - 1)];
}
