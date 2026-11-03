/*
 * A023224  Primes p such that 7*p + 4 is also prime.
 * https://oeis.org/A023224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that 7*p + 4 is also prime
 * Range:     u8 n <= 11, u16 n <= 876, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9843 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t term_count = 0;
    static size_t term_cap = 0;
    static idx_t last_checked = 1;

    while (term_count < n) {
        ++last_checked;
        if (!is_prime(last_checked))
            continue;
        idx_t qp = 7 * last_checked + 4;
        if (is_prime(qp)) {
            grow_seq(&terms, &term_cap, term_count + 1);
            terms[term_count++] = (seq_t)last_checked;
        }
    }
    return terms[n - 1];
}
