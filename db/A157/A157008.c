/*
 * A157008  Four-digit primes that have at least 4 permutations (without
 *          leading zeros) that are primes.
 * https://oeis.org/A157008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the n-th 4‑digit prime that has at least four * distinct
 *            prime permutations (no leading zeros), and for each * multiset
 *            of digits only the smallest prime with that multiset * is
 *            included. The sequence is generated once by a sieve up to * 9999
 *            and stored in a static array
 * Range:     u8 none, u16 n <= 123, u32 n <= 123, u64 n <= 123, u128 n <= 123,
 *            big any n
 * Helpers:   append_candidate, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    /* Static storage for the sequence, generated on first call. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Sieve primes up to 9999. */
        static bool is_comp[10000] = {0};
        for (size_t i = 2; i * i < 10000; ++i) {
            if (!is_comp[i]) {
                for (size_t j = i * i; j < 10000; j += i) {
                    is_comp[j] = true;
                }
            }
        }

        /* Temporary dynamic array for the primes that qualify. */
        seq_t *arr = NULL;
        size_t len = 0, cap = 0;

        /* Global flag for each 4-digit multiset. 0 = unseen, 1 = seen.  */
        static bool seen_multiset[10000] = {0};

        /* Helper array to avoid counting duplicate permutations for a prime. */
        static uint16_t seen_perm[10000] = {0};
        static uint16_t perm_tag = 1;

        /* Process all 4‑digit primes in ascending order. */
        for (int p = 1000; p < 10000; ++p) {
            if (is_comp[p]) continue;          /* not a prime */

            /* Compute the canonical representation (sorted digits). */
            int d[4];
            d[0] = p / 1000;
            d[1] = (p / 100) % 10;
            d[2] = (p / 10) % 10;
            d[3] = p % 10;

            /* Simple selection sort for 4 digits. */
            for (int i = 0; i < 3; ++i) {
                for (int j = i + 1; j < 4; ++j) {
                    if (d[j] < d[i]) {
                        int tmp = d[i];
                        d[i] = d[j];
                        d[j] = tmp;
                    }
                }
            }
            int canonical = d[0] * 1000 + d[1] * 100 + d[2] * 10 + d[3];

            /* If this multiset has already produced a prime, skip it. */
            if (seen_multiset[canonical]) {
                continue;
            }

            /* Count distinct prime permutations of this prime. */
            ++perm_tag;
            size_t prime_count = 0;

            for (int i0 = 0; i0 < 4; ++i0) {
                if (d[i0] == 0) continue;          /* leading zero not allowed */
                int num0 = d[i0];
                for (int i1 = 0; i1 < 4; ++i1) if (i1 != i0) {
                    int num1 = d[i1];
                    for (int i2 = 0; i2 < 4; ++i2) if (i2 != i0 && i2 != i1) {
                        int num2 = d[i2];
                        for (int i3 = 0; i3 < 4; ++i3) if (i3 != i0 && i3 != i1 && i3 != i2) {
                            int num3 = d[i3];
                            int val = num0 * 1000 + num1 * 100 + num2 * 10 + num3;
                            if (seen_perm[val] == perm_tag) continue;
                            seen_perm[val] = perm_tag;
                            if (is_prime_small((seq_t)val))
                                ++prime_count;
                        }
                    }
                }
            }

            /* Record that this multiset has been processed. */
            seen_multiset[canonical] = true;

            if (prime_count >= 4) {
                append_candidate(&arr, &len, &cap, (seq_t)p);
            }
        }

        seq = arr;
        seq_len = len;
        initialized = true;
    }

    if (n == 0 || (size_t)(n - 1) >= seq_len) {
        return 0;
    }
    return seq[n - 1];
}
