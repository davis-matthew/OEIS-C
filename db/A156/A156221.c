/*
 * A156221  Primes that can be represented with the digits {3,1,4,1,5,9}.
 * https://oeis.org/A156221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 143, u32 n <= 221, u64 n <= 221,
 *            u128 n <= 221, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A156221 (primes made from the digits 3,1,4,1,5,9,
   with the digit 1 allowed at most twice and each of 3,4,5,9 at most once). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* maximum number of primes in the sequence is 221, use a safety margin */
static uint64_t primes_storage[256];
static size_t primes_count = 0;

/* compare two uint64_t values for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* simple primality test for numbers <= 10^6 */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* recursive generation of all numbers with given digit counts */
static void generate_recursive(size_t total, size_t depth,
                               unsigned count[5], const int digits[5],
                               uint64_t value)
{
    if (depth == total) {                     /* number complete */
        if (is_prime_uint64(value))
            primes_storage[primes_count++] = value;
        return;
    }
    for (int i = 0; i < 5; ++i) {
        if (count[i] == 0) continue;
        --count[i];
        generate_recursive(total, depth + 1, count, digits,
                           value * 10 + (uint64_t)digits[i]);
        ++count[i];
    }
}

/* build the list of primes once */
static void init_sequence(void)
{
    const int digits[5] = {1, 3, 4, 5, 9};
    unsigned count[5];

    for (int c1 = 0; c1 <= 2; ++c1)              /* 1 used 0,1,2 times */
        for (int c3 = 0; c3 <= 1; ++c3)          /* 3 used 0,1 times  */
            for (int c4 = 0; c4 <= 1; ++c4)      /* 4 used 0,1 times  */
                for (int c5 = 0; c5 <= 1; ++c5)  /* 5 used 0,1 times  */
                    for (int c9 = 0; c9 <= 1; ++c9) { /* 9 used 0,1 times */
                        int total = c1 + c3 + c4 + c5 + c9;
                        if (total == 0) continue;  /* no digits */
                        count[0] = (unsigned)c1;
                        count[1] = (unsigned)c3;
                        count[2] = (unsigned)c4;
                        count[3] = (unsigned)c5;
                        count[4] = (unsigned)c9;
                        generate_recursive((size_t)total, 0, count,
                                           digits, 0);
                    }
    /* sort the primes so that a(n) returns them in increasing order */
    qsort(primes_storage, primes_count, sizeof(uint64_t), cmp_uint64);
}

/* ------------------------------------------------------------- */

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_sequence();
        initialized = true;
    }
    if (n < 1 || (size_t)n > primes_count)
        return 0;                              /* out of bounds – return 0 */
    return (seq_t)primes_storage[(size_t)n - 1];
}
