/*
 * A031951  Numbers with exactly two distinct base-6 digits.
 * https://oeis.org/A031951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number with exactly two distinct base‑6 digits (OEIS A031951)
 * Range:     u8 n <= 105, u16 n <= 1531, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison routine for qsort */
static int seq_compare(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* dynamic array to store generated values, kept sorted */
static seq_t *values = NULL;
static size_t values_size = 0;
static size_t values_cap = 0;
static idx_t max_len_generated = 1;   /* lengths generated up to max_len_generated‑1 */

/* add a value to the array, expanding if necessary */
static void append_value(seq_t v)
{
    if (values_size == values_cap) {
        values_cap = (values_cap == 0) ? 16 : values_cap * 2;
        seq_t *tmp = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        values = tmp;
    }
    values[values_size++] = v;
}

/* generate all numbers with exactly two distinct base‑6 digits of a given length L */
static void generate_length(idx_t L)
{
    size_t bits = (size_t)(L - 1);            /* number of positions after the leading digit */
    size_t patterns = (size_t)1 << bits;     /* 2^(L-1) patterns */

    for (int d1 = 0; d1 <= 5; ++d1) {
        for (int d2 = d1 + 1; d2 <= 5; ++d2) {
            /* pair {d1,d2} */
            for (int lead = 0; lead <= 1; ++lead) {
                int ld   = (lead == 0) ? d1 : d2;   /* leading digit */
                int od   = (lead == 0) ? d2 : d1;   /* the other digit */
                if (ld == 0) continue;              /* leading zero not allowed */

                for (size_t mask = 0; mask < patterns; ++mask) {
                    /* skip the pattern where all remaining digits equal the leading digit */
                    if (mask == patterns - 1) continue;

                    seq_t val = (seq_t)ld;
                    for (idx_t pos = 1; pos < L; ++pos) {
                        size_t shift = bits - pos;
                        int bit = (int)((mask >> shift) & 1);
                        int digit = bit ? ld : od;
                        val = val * (seq_t)6 + (seq_t)digit;
                    }
                    append_value(val);
                }
            }
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1, undefined for n = 0 */

    /* generate more terms if necessary */
    size_t cumulative = values_size;
    idx_t L = max_len_generated;
    while (cumulative < n) {
        ++L;
        generate_length(L);
        cumulative = values_size;  /* updated after each length */
    }
    max_len_generated = L;

    /* keep the array sorted */
    qsort(values, values_size, sizeof(seq_t), seq_compare);

    return values[n - 1];
}
