/*
 * A134966  Primes that use all of the prime digits 2,3,5,7 exactly once.
 * https://oeis.org/A134966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime that uses each of the digits 2,3,5,7 exactly once */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* generate all primes containing the digits 2,3,5,7 exactly once */
static void generate_primes(seq_t *out, size_t *cnt) {
    const int d[4] = {2, 3, 5, 7};
    seq_t temp[24];
    size_t sz = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) if (j != i) {
            for (int k = 0; k < 4; ++k) if (k != i && k != j) {
                for (int l = 0; l < 4; ++l) if (l != i && l != j && l != k) {
                    seq_t val = d[i];
                    val = val * 10 + d[j];
                    val = val * 10 + d[k];
                    val = val * 10 + d[l];
                    if (is_prime_big(val)) {
                        temp[sz++] = val;
                    }
                }
            }
        }
    }

    /* simple insertion sort for at most 8 elements */
    for (size_t m = 1; m < sz; ++m) {
        seq_t key = temp[m];
        size_t n = m;
        while (n > 0 && temp[n - 1] > key) {
            temp[n] = temp[n - 1];
            --n;
        }
        temp[n] = key;
    }

    for (size_t i = 0; i < sz; ++i) out[i] = temp[i];
    *cnt = sz;
}

seq_t a(idx_t n) {
    static seq_t primes[24];
    static size_t prime_cnt = 0;
    if (prime_cnt == 0) {
        generate_primes(primes, &prime_cnt);
    }
    if (n == 0 || n > prime_cnt) return 0;
    return primes[n - 1];
}
