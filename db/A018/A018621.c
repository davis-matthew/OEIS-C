/*
 * A018621  Divisors of 741.
 * https://oeis.org/A018621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th divisor of 741 in ascending order
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static bool initialized = false;
static seq_t divisors[8];

/* Generate all divisors of 741 by prime factorization.
   741 = 3 * 13 * 19 (each exponent is 1). */
static void generate_divisors(const uint64_t primes[], const int exps[],
                              size_t prime_cnt, seq_t current, size_t idx,
                              seq_t *out, size_t *cnt)
{
    if (idx == prime_cnt) {
        out[(*cnt)++] = current;
        return;
    }
    uint64_t p = primes[idx];
    int exp = exps[idx];
    seq_t val = current;
    for (int e = 0; e <= exp; ++e) {
        generate_divisors(primes, exps, prime_cnt, val, idx + 1, out, cnt);
        val *= (seq_t)p;
    }
}

/* Simple insertion sort for 8 elements. */
static void sort_divisors(seq_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Factor 741: 3 * 13 * 19 */
        const uint64_t primes[3] = {3ULL, 13ULL, 19ULL};
        const int exps[3] = {1, 1, 1};
        size_t cnt = 0;
        generate_divisors(primes, exps, 3, (seq_t)1, 0, divisors, &cnt);
        sort_divisors(divisors, cnt);
        initialized = true;
    }
    /* Offset is 1, so a(1) is the first divisor. */
    if (n >= 1 && n <= 8) {
        return divisors[(size_t)(n - 1)];
    }
    /* For indices beyond the finite list, return 0. */
    return (seq_t)0;
}
