/*
 * A085451  Numbers n such that n and prime[n] together use only distinct
 *          digits.
 * https://oeis.org/A085451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A085451:  n such that n and the nth prime use only distinct digits */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/distinct_digits.h"

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t terms[101];
    static size_t term_count = 0;

    if (!initialized) {
        /* constants */
        static const idx_t MAX_PRIMES = 3106;       /* last needed prime index */
        static const int SIEVE_LIMIT = 40000;       /* > 3106th prime */

        /* simple sieve of Eratosthenes */
        char *is_comp = (char*)calloc(SIEVE_LIMIT + 1, 1);
        if (!is_comp) return 0; /* allocation failure */

        for (int i = 2; i * i <= SIEVE_LIMIT; ++i) {
            if (!is_comp[i]) {
                for (int j = i * i; j <= SIEVE_LIMIT; j += i) {
                    is_comp[j] = 1;
                }
            }
        }

        /* collect the first MAX_PRIMES primes */
        uint64_t *prime_list = (uint64_t*)calloc(MAX_PRIMES, sizeof(uint64_t));
        if (!prime_list) {
            free(is_comp);
            return 0;
        }
        size_t pcnt = 0;
        for (int i = 2; i <= SIEVE_LIMIT && pcnt < MAX_PRIMES; ++i) {
            if (!is_comp[i]) {
                prime_list[pcnt++] = (uint64_t)i;
            }
        }

        /* compute the sequence terms (at most 101 of them) */
        size_t cnt = 0;
        for (idx_t i = 1; i <= MAX_PRIMES && cnt < 101; ++i) {
            uint64_t p = prime_list[i - 1];
            if (distinct_digits(i, p)) {
                terms[cnt++] = (seq_t)i;
            }
        }
        term_count = cnt;
        initialized = true;

        /* free temporary storage */
        free(is_comp);
        free(prime_list);
    }

    if (n == 0) return 0;
    if (n <= term_count) {
        return terms[n - 1];
    }
    return 0;   /* n beyond known terms */
}
