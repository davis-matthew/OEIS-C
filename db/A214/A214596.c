/*
 * A214596  Number of 3X3X3 triangular 0..n arrays with every horizontal row
 *          having the same average value.
 * https://oeis.org/A214596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) - 3*a(n-4) - 6*a(n-5) * + 6*a(n-7) +
 *            3*a(n-8) - 2*a(n-9) - 3*a(n-10) + a(n-12), * for n > 12; a(1)=2,
 *            a(2)=23, a(3)=62, a(4)=157, a(5)=312, * a(6)=601, a(7)=986,
 *            a(8)=1619, a(9)=2426, a(10)=3589, * a(11)=5028, a(12)=6997. All
 *            indices are 1‑based
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* out of offset */

    /* pre‑compensated prefix of length 12 */
    static const seq_t base[13] = {0ULL, /* placeholder for 0 index */
        2ULL, 23ULL, 62ULL, 157ULL, 312ULL, 601ULL, 986ULL,
        1619ULL, 2426ULL, 3589ULL, 5028ULL, 6997ULL};

    if (n <= 12) return base[n];

    /* Allocate an array large enough to keep indices 1..n.
       Index 0 remains unused, so that references such as a[i-2]
       stay valid. */
    seq_t *arr = (seq_t *)calloc((size_t)n + 13, sizeof(seq_t));
    if (!arr) return 0;  /* allocation failure, return 0 as safe fail */

    /* Copy the known first 12 terms. */
    for (idx_t i = 1; i <= 12; ++i) arr[i] = base[i];

    /* Compute subsequent terms using the recurrence. */
    for (idx_t i = 13; i <= n; ++i) {
        /* Arrange positive and negative contributions separately
           so that no intermediate subtraction underflows an
           unsigned seq_t. */
        seq_t sum_pos = arr[i-2] * 3;
        sum_pos += arr[i-3] * 2;
        sum_pos += arr[i-7] * 6;
        sum_pos += arr[i-8] * 3;
        sum_pos += arr[i-12];

        seq_t sum_neg = arr[i-4] * 3;
        sum_neg += arr[i-5] * 6;
        sum_neg += arr[i-9] * 2;
        sum_neg += arr[i-10] * 3;

        arr[i] = sum_pos - sum_neg;   /* mathematically safe */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
