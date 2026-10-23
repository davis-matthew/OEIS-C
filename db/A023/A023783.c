/*
 * A023783  Metadromes: digits in base 15 are in strict ascending order.
 * https://oeis.org/A023783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 6788, u64 n <= 8125,
 *            u128 n <= 8125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Metadromes – numbers whose base‑15 representation has strictly increasing digits.
   The function enumerates all subsets of the digits 1..14, forms the
   corresponding base‑15 number, and sorts the resulting list.  The
   n-th element (n ≥ 1) is returned.  For n > 16384 (when the
   sequence is exhausted) 0 is returned. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_COUNT 16384  /* 2^14, number of possible combinations */

static seq_t values[MAX_COUNT];
static bool initialized = false;

static void insertion_sort(seq_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* generate all increasing-digit numbers in base 15 */
        values[0] = (seq_t)0;   /* 0 is the only number with a leading zero digit */
        size_t idx = 1;
        for (uint64_t mask = 1; mask < (uint64_t)MAX_COUNT; ++mask) {
            seq_t val = (seq_t)0;
            for (int d = 1; d <= 14; ++d) {
                if (mask & ((uint64_t)1 << (d - 1))) {
                    val = val * (seq_t)15 + (seq_t)d;
                }
            }
            values[idx++] = val;
        }
        insertion_sort(values, (size_t)MAX_COUNT);
        initialized = true;
    }

    if (n == 0 || n > (idx_t)MAX_COUNT)
        return (seq_t)0;               /* sequence undefined beyond MAX_COUNT */

    return values[(size_t)(n - 1)];
}
