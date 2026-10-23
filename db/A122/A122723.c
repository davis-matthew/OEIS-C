/*
 * A122723  Primes that are the sum of three distinct positive cubes.
 * https://oeis.org/A122723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate all primes up to an upper bound and mark those that are
   the sum of three distinct positive cubes.  Then return the n‑th such prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n) {
    /* static state to cache the sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static bool initialized = false;

    /* Helper to free memory */
    #define FREE(ptr) do { if (ptr) { free(ptr); ptr = NULL; } } while(0)

    /* Pick a generous initial upper bound.  20 million covers well
       beyond the first 10 000 terms. */
    const idx_t LIMIT = 20000000;
    const idx_t SQRT_LIMIT = (idx_t)floor(sqrt((double)LIMIT));

    /* Precompute only once */
    if (!initialized) {
        /* Allocate space for the resulting sequence (estimate). */
        seq_cap = 20000;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0; /* out of memory – never expected */

        /* Sieve of Eratosthenes */
        uint8_t *is_prime = (uint8_t *)malloc((size_t)(LIMIT + 1));
        if (!is_prime) return 0;
        for (size_t i = 0; i <= (size_t)LIMIT; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;
        for (idx_t i = 2; i <= SQRT_LIMIT; ++i) {
            if (is_prime[(size_t)i]) {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    is_prime[(size_t)j] = 0;
            }
        }

        /* Mark sums of three distinct positive cubes */
        uint8_t *sum_flag = (uint8_t *)calloc((size_t)(LIMIT + 1), 1);
        if (!sum_flag) { FREE(is_prime); return 0; }

        int cube_root = (int)floor(cbrt((double)LIMIT)) + 1;   /* ceil */
        for (int a = 1; a <= cube_root; ++a) {
            seq_t a3 = (seq_t)a * a * a;
            for (int b = a + 1; b <= cube_root; ++b) {
                seq_t b3 = (seq_t)b * b * b;
                seq_t ab = a3 + b3;
                if (ab > LIMIT) break;
                for (int c = b + 1; c <= cube_root; ++c) {
                    seq_t sum = ab + (seq_t)c * c * c;
                    if (sum > LIMIT) break;
                    sum_flag[(size_t)sum] = 1;
                }
            }
        }

        /* Collect primes that are sums of three distinct cubes */
        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (is_prime[(size_t)i] && sum_flag[(size_t)i]) {
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)i;
                if (seq_len >= 100000) break;   /* enough for all tests */
            }
        }

        FREE(is_prime);
        FREE(sum_flag);
        initialized = true;
    }

    /* Return the requested term (1‑based index) */
    if (n == 0 || n > seq_len) return 0; /* out of bounds */
    return seq[(size_t)(n - 1)];
}
