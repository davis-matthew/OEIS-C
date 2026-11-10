/*
 * A092729  Duplicate of A072667.
 * https://oeis.org/A092729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = prime(m) such that prime(m) + prime(m+1) = prime(k) + 1
 *            for some k; computed by search over consecutive primes
 *            (duplicate of A072667)
 * Range:     i8 n <= 16, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static seq_t A072667_a(idx_t n)
{
    if (n == 0) return 0;

    static uint64_t candidate = 2;
    static uint64_t prev_prime = 0;
    static seq_t *results = NULL;
    static size_t results_len = 0;
    static size_t results_cap = 0;

    while (results_len < n)
    {
        uint64_t p;

        if (candidate == 2) {
            p = 2;
            candidate = 3;
        } else {
            if ((candidate & 1) == 0)
                candidate++;
            while (!is_prime(candidate))
                candidate += 2;
            p = candidate;
            candidate += 2;
        }

        if (prev_prime != 0) {
            uint64_t sum = prev_prime + p;
            if (is_prime(sum - 1)) {
                if (results_len == results_cap) {
                    size_t new_cap = results_cap ? results_cap * 2 : 8;
                    seq_t *tmp = mem_resize(seq_t, results, new_cap);
                    if (!tmp) {
                        exit(1);
                    }
                    results = tmp;
                    results_cap = new_cap;
                }
                results[results_len++] = (seq_t)prev_prime;
            }
        }

        prev_prime = p;
    }

    return results[n - 1];
}

seq_t a(idx_t n)
{
    return A072667_a(n);
}
