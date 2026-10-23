/*
 * A153024  a(n) is the number of iterations of the map k -> A048050(k) to
 *          reach zero. If we never reach 0, then a(n) = -1. A048050 gives the
 *          sum of proper divisors of k, excluding both 1 and n from the sum.
 * https://oeis.org/A153024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 + a(A048050(n)) until a zero is reached, else a(n) =
 *            -1. A048050(n) = sum of all proper divisors of n excluding 1 and
 *            n
 * Range:     i8 n <= 83, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute the sum of ALL divisors of n (including 1 and n). */
static inline seq_t sum_of_divisors(idx_t n) {
    if (n == 1) return 1;          /* divisors of 1 are just {1} */

    seq_t sigma = 1;               /* σ(n) */
    idx_t tmp = n;

    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            seq_t pow_p = 1;       /* p^e  (initially p^0) */
            seq_t sum_p = 1;       /* 1 + p + p² + ... + p^e */

            while (tmp % p == 0) {
                tmp /= p;
                pow_p *= p;
                sum_p += pow_p;
            }
            sigma *= sum_p;
        }
    }
    if (tmp > 1) {                 /* remaining prime factor */
        sigma *= (1 + tmp);
    }
    return sigma;
}

/* Sum of proper divisors of n excluding 1 and n.
   For n ≤ 2 the set is empty, so the sum is 0. */
static inline seq_t sum_nontrivial_divisors(idx_t n) {
    if (n <= 2) return 0;

    seq_t sigma = sum_of_divisors(n);
    seq_t result = sigma - 1 - (seq_t)n;   /* subtract 1 and n */
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1, 0 is unused */

    idx_t cur = n;
    seq_t steps = 0;

    /* Simple visited set to detect cycles. */
    size_t cap = 8, cnt = 0;
    idx_t *visited = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!visited) return -1;        /* allocation failure */

    while (1) {
        if (cur == 0) {
            free(visited);
            return steps;
        }

        /* look for repetition */
        for (size_t i = 0; i < cnt; ++i) {
            if (visited[i] == cur) {
                free(visited);
                return -1;
            }
        }

        /* grow visited array if needed */
        if (cnt == cap) {
            cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(visited, cap * sizeof(idx_t));
            if (!tmp) {
                free(visited);
                return -1;
            }
            visited = tmp;
        }
        visited[cnt++] = cur;

        /* next iteration */
        seq_t next_val = sum_nontrivial_divisors(cur);
        cur = (idx_t)next_val;
        ++steps;
    }
}
