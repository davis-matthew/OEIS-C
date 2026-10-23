/*
 * A112131  Concatenate the consecutive increasing numbers starting with
 *          Harshad(n) and ending with Harshad(n+1).
 * https://oeis.org/A112131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat_{k=Harshad(n)}^{Harshad(n+1)} k
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 31, u128 n <= 101,
 *            big any n
 * Helpers:   count_digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/count_digits.h"
#include "../utils/pow10.h"

static idx_t *harshad_arr = NULL;   /* array of Harshad numbers   */
static size_t harshad_cap = 0;      /* allocated capacity        */
static size_t harshad_cnt = 0;      /* number of stored elements */
static idx_t  last_checked = 0;     /* last integer examined */

/* Ensure that harshad_arr contains at least target Harshad numbers. */
static void
ensure_harshads(size_t target)
{
    if (harshad_cnt >= target)
        return;

    /* Expand the array if necessary. */
    if (harshad_cap < target) {
        size_t new_cap = harshad_cap ? harshad_cap * 2 : 128;
        while (new_cap < target)
            new_cap *= 2;
        idx_t *tmp = (idx_t *)realloc(harshad_arr, new_cap * sizeof(idx_t));
        if (!tmp) exit(1);           /* out of memory */
        harshad_arr = tmp;
        harshad_cap = new_cap;
    }

    /* Generate Harshad numbers until we have target elements. */
    while (harshad_cnt < target) {
        ++last_checked;
        idx_t y   = last_checked;
        idx_t sum = 0;
        while (y > 0) {
            sum += y % 10;
            y   /= 10;
        }
        if (sum != 0 && last_checked % sum == 0) {
            harshad_arr[harshad_cnt++] = last_checked;
        }
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for 0, just return 0 */

    /* We need Harshad(n) and Harshad(n+1). */
    ensure_harshads((size_t)n + 1);

    idx_t start = harshad_arr[n - 1];
    idx_t end   = harshad_arr[n];

    seq_t result = 0;
    for (idx_t k = start; k <= end; ++k) {
        int d   = count_digits(k);
        seq_t mult = pow10((idx_t)d);           /* 10^{#digits(k)} */
        result = result * mult + (seq_t)k;
    }
    return result;
}
