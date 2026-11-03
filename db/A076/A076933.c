/*
 * A076933  Final number obtained when n is divided by its divisors starting
 *          from the smallest one in increasing order until one no longer gets
 *          an integer.
 * https://oeis.org/A076933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 386, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 115 terms through n = 9910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076933: the final number obtained when n is divided by its
   divisors in increasing order. */
#include <stdint.h>
#include <stddef.h>

/* Simple insertion sort for the small divisor array. */
static void sort_divisors(uint64_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        uint64_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

/* Maximum number of divisors expected for the n values considered
   (overkill for n up to ~10^6). */
#define MAX_DIVISORS 256

/* Compute A076933(n). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n ≥ 1 */

    uint64_t divisors[MAX_DIVISORS];
    size_t dcnt = 0;

    /* Find all divisors of n.  We iterate i up to n/i to avoid overflow. */
    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (dcnt < MAX_DIVISORS) divisors[dcnt++] = i;
            uint64_t j = n / i;
            if (j != i && dcnt < MAX_DIVISORS) divisors[dcnt++] = j;
        }
    }

    /* Sort the divisors increasingly. */
    sort_divisors(divisors, dcnt);

    /* Perform the division process using seq_t arithmetic. */
    seq_t current = (seq_t)n;
    for (size_t k = 0; k < dcnt; ++k) {
        uint64_t d = divisors[k];
        if (current % (seq_t)d == (seq_t)0) {
            current = current / (seq_t)d;
        } else {
            break;
        }
    }

    return current;
}
