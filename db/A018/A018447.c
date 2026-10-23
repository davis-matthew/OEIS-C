/*
 * A018447  Divisors of 425.
 * https://oeis.org/A018447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists the divisors of 425 in ascending order. */
#include <stddef.h>
#include <stdbool.h>

/* Helper to sort a small array of seq_t values. */
static void sort_seq_t(seq_t *arr, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j + 1 < n - i; ++j)
            if (arr[j] > arr[j + 1]) {
                seq_t tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
}

seq_t a(idx_t n)
{
    /* Divisors of 425 = 5² × 17.  The result list is 1,5,17,25,85,425. */
    static seq_t divisors[6];
    static bool initialized = false;

    if (!initialized) {
        size_t pos = 0;
        for (int a = 0; a <= 2; ++a) {
            seq_t val5 = 1;
            for (int i = 0; i < a; ++i) val5 *= 5;
            for (int b = 0; b <= 1; ++b) {
                seq_t val = val5;
                for (int i = 0; i < b; ++i) val *= 17;
                divisors[pos++] = val;
            }
        }
        sort_seq_t(divisors, 6);
        initialized = true;
    }

    if (n >= 1 && n <= 6)
        return divisors[(size_t)(n - 1)];
    return (seq_t)0; /* undefined for n > 6 */
}
