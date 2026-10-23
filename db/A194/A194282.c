/*
 * A194282  Variant of the even lucky numbers (A045954).
 * https://oeis.org/A194282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variant of even lucky numbers – sieve algorithm. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n < 1 */

    /* Empirical upper bound for n ≤ 10000: */
/*    n * 200 + 100 evens are enough to keep at least n survivors after sieving. */
    size_t initial = (size_t)n * 200 + 100;

    seq_t *arr = (seq_t *)malloc(initial * sizeof(seq_t));
    if (!arr) return (seq_t)0;               /* allocation failure */

    for (size_t i = 0; i < initial; ++i)
        arr[i] = (seq_t)(2 * (i + 1));       /* generate initial list of evens */

    size_t list_size = initial;
    size_t step_idx  = 0;                    /* index of current sieving step */

    while (step_idx < list_size) {
        seq_t step_val = arr[step_idx];
        if (step_val > (seq_t)n) break;      /* later steps cannot affect first n */

        size_t new_size = 0;
        size_t step = (size_t)step_val;
        for (size_t j = 0; j < list_size; ++j)
            if (((j + 1) % step) != 0)
                arr[new_size++] = arr[j];

        list_size = new_size;
        ++step_idx;
    }

    seq_t result = arr[n - 1];              /* zero‑based index */
    free(arr);
    return result;
}
