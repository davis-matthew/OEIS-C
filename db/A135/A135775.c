/*
 * A135775  Numbers having number of divisors equal to number of digits in
 *          base 5.
 * https://oeis.org/A135775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   no closed form; a(n) is the nth positive integer whose number of
 *            * divisors equals its number of digits in base 5
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   base5_digit_count, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/base5_digit_count.h"

/* static storage for memoized terms */
static seq_t *terms = NULL;
static size_t terms_count = 0;
static size_t terms_cap = 0;
static uint64_t last_checked = 0;

/* ensure the terms array can hold at least n elements */
static void ensure_capacity(size_t n)
{
    if (terms_cap >= n)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 128;
    while (new_cap < n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* generate terms until index n is reached */
static void compute_terms_upto(idx_t n)
{
    if (n == 0)
        return;
    size_t target = (size_t)n;
    if (terms_count >= target)
        return;

    ensure_capacity(target);

    for (uint64_t i = last_checked + 1; terms_count < target; ++i)
    {
        last_checked = i;
        uint64_t divc = divisor_count_u64(i);
        if (divc == (uint64_t)base5_digit_count(i))
        {
            terms[terms_count++] = (seq_t)i;
        }
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n <= 0 */

    compute_terms_upto(n);
    return terms[(size_t)(n - 1)];
}
