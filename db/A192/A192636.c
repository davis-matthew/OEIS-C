/*
 * A192636  Powerful sums of two powerful numbers.
 * https://oeis.org/A192636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 480, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A192636: Powerful sums of two powerful numbers.
   The function generates all powerful numbers up to a fixed limit,
   then selects those that can be expressed as the sum of two
   (not necessarily distinct) powerful numbers. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* assumes seq_t and idx_t are defined by the build */
extern seq_t a(idx_t n);
static bool initialized = false;
static seq_t *powers = NULL;      /* array of all powerful numbers */
static size_t power_cnt = 0;
static seq_t *seq_arr = NULL;     /* array of sequence terms */
static size_t seq_len = 0;

/* generate all powerful numbers up to LIMIT and compute the sequence */
static void compute_sequence(void)
{
    const idx_t LIMIT = 35000000ULL;  /* sufficient for n up to ~10000 */
    bool *is_powerful = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_powerful) exit(1);
    for (idx_t i = 0; i <= LIMIT; ++i) is_powerful[i] = false;

    /* 1 is powerful */
    is_powerful[1] = true;

    /* generate numbers of the form a^2 * b^3 */
    for (idx_t a = 1; a * a <= LIMIT; ++a) {
        idx_t a2 = a * a;
        for (idx_t b = 1;; ++b) {
            idx_t b3 = b * b * b;
            idx_t val = a2 * b3;
            if (val > LIMIT) break;
            is_powerful[val] = true;
        }
    }

    /* count powerful numbers */
    for (idx_t i = 1; i <= LIMIT; ++i)
        if (is_powerful[i]) ++power_cnt;

    /* allocate arrays */
    powers = (seq_t *)malloc(power_cnt * sizeof(seq_t));
    seq_arr = (seq_t *)malloc(power_cnt * sizeof(seq_t));  /* at most
                                                                power_cnt terms */
    if (!powers || !seq_arr) exit(1);

    /* fill powers array in sorted order */
    size_t idx_p = 0;
    for (idx_t i = 1; i <= LIMIT; ++i)
        if (is_powerful[i]) powers[idx_p++] = (seq_t)i;

    /* build the sequence: powerful numbers that can be expressed
       as the sum of two powerful numbers */
    size_t seq_idx = 0;
    for (size_t i = 0; i < power_cnt; ++i) {
        seq_t c = powers[i];
        if (c == 1) continue;        /* cannot be expressed as sum */
        bool found = false;
        for (size_t j = 0; j <= i && powers[j] <= c / (seq_t)2; ++j) {
            seq_t a = powers[j];
            seq_t b = c - a;
            if (b <= LIMIT && is_powerful[(idx_t)b]) {
                found = true;
                break;
            }
        }
        if (found) seq_arr[seq_idx++] = c;
    }
    seq_len = seq_idx;   /* number of terms computed */

    /* clean up */
    free(is_powerful);
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_sequence();
        initialized = true;
    }
    /* n is 1‑based; the function is only defined for n >= 1 */
    return seq_arr[(size_t)(n - 1)];
}
