/*
 * A126245  (p+2)! if p prime and p+2 prime.
 * https://oeis.org/A126245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   factorial, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126245: factorial of the n‑th upper twin prime */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against 0 */
        return (seq_t)0;

    /* Cache of the upper twin primes found so far */
    static uint64_t *upper = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Candidate for the lower twin prime; start at 3 */
    static uint64_t cand = 3;

    /* Keep searching until we have at least n upper primes */
    while (len < n) {
        /* Skip even numbers */
        if ((cand & 1ULL) == 0)
            ++cand;

        if (is_prime(cand)) {
            uint64_t up = cand + 2;
            if (is_prime(up)) {
                /* Grow cache if necessary */
                if (len >= cap) {
                    cap = (cap == 0) ? 4 : cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(upper, cap * sizeof(uint64_t));
                    if (!tmp)  /* allocation failed */
                        return (seq_t)0;
                    upper = tmp;
                }
                upper[len++] = up;
            }
        }
        ++cand;
    }

    /* Return factorial of the n‑th upper twin prime (1‑based) */
    return factorial((idx_t)upper[n - 1]);
}
