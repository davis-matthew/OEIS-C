/*
 * A168349  Numbers of the form a^b + c^d where a, b, c and d are the first 4
 *          primes.
 * https://oeis.org/A168349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   power_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168349: numbers of the form a^b + c^d with a,b,c,d a permutation
   of the first four primes {2,3,5,7}.  The sequence is defined by all
   distinct sums, sorted in increasing order. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/power_ull.h"

static const seq_t primes[4] = {2, 3, 5, 7};

static void generate_sequence(void) {
    seq_t values[24];
    size_t count = 0;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) if (j != i) {
            for (int k = 0; k < 4; ++k) if (k != i && k != j) {
                for (int l = 0; l < 4; ++l) if (l != i && l != j && l != k) {
                    unsigned long long p1 = power_ull((unsigned long long)primes[i], (int)primes[j]);
                    unsigned long long p2 = power_ull((unsigned long long)primes[k], (int)primes[l]);
                    seq_t sum = (seq_t)(p1 + p2);
                    values[count++] = sum;
                }
            }
        }
    }

    /* sort the 24 values (insertion sort, since the array is tiny) */
    for (size_t m = 1; m < count; ++m) {
        seq_t key = values[m];
        size_t n = m;
        while (n > 0 && values[n - 1] > key) {
            values[n] = values[n - 1];
            --n;
        }
        values[n] = key;
    }

    /* deduplicate into the final static array */
    static seq_t seq[12];
    static bool inited = false;
    size_t pos = 0;
    if (pos < count) {
        seq[pos++] = values[0];
        for (size_t m = 1; m < count; ++m) {
            if (values[m] != values[m - 1]) {
                if (pos < 12) seq[pos++] = values[m];
            }
        }
    }
    inited = true;
}

seq_t a(idx_t n) {
    static bool generated = false;
    static seq_t seq[12];
    if (!generated) {
        /* generate the sequence once */
        static const seq_t primes[4] = {2, 3, 5, 7};
        seq_t values[24];
        size_t count = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) if (j != i) {
                for (int k = 0; k < 4; ++k) if (k != i && k != j) {
                    for (int l = 0; l < 4; ++l) if (l != i && l != j && l != k) {
                        unsigned long long p1 = power_ull((unsigned long long)primes[i], (int)primes[j]);
                        unsigned long long p2 = power_ull((unsigned long long)primes[k], (int)primes[l]);
                        values[count++] = (seq_t)(p1 + p2);
                    }
                }
            }
        }
        for (size_t m = 1; m < count; ++m) {
            seq_t key = values[m];
            size_t n = m;
            while (n > 0 && values[n - 1] > key) {
                values[n] = values[n - 1];
                --n;
            }
            values[n] = key;
        }
        size_t pos = 0;
        seq[pos++] = values[0];
        for (size_t m = 1; m < count; ++m) {
            if (values[m] != values[m - 1]) {
                seq[pos++] = values[m];
            }
        }
        generated = true;
    }
    if (n == 0 || n > 12)
        return 0;
    return seq[(size_t)(n - 1)];
}
