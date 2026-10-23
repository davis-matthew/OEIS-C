/*
 * A138142  Nonnegative numbers with digits in descending order that differ
 *          exactly by 1.
 * https://oeis.org/A138142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, nonn
 * Formula:   a(n) is the number consisting of consecutive descending digits,
 *            enumerated first by increasing length (1‑digit numbers 0–9),
 *            then by increasing starting digit. For length k the starting
 *            digit d ranges from k-1 up to 9, yielding the numbers
 *            d(d-1)...(d-k+1)
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Example: n=11 → k=2, d=1 → 10; n=24 → k=3, d=6 → 654. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Number of terms of each length: 1‑digit:10, 2‑digit:9, …, 10‑digit:1 */
    static const idx_t counts[10] = {10,9,8,7,6,5,4,3,2,1};

    /* Convert to 0‑based index */
    idx_t idx = n - 1;
    idx_t k = 1;                     /* length of the number */

    /* Find the length k for this index */
    while (k <= 10 && idx >= counts[k-1]) {
        idx -= counts[k-1];
        ++k;
    }

    if (k > 10)            /* n is outside the finite sequence */
        return (seq_t)0;   /* undefined behaviour – return 0 */

    /* Offset within the k‑digit block */
    idx_t offset = idx;                      /* 0‑based within this block */
    idx_t start_digit = (k - 1) + offset;    /* the first (highest) digit */

    /* Build the number by appending decreasing digits */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i) {
        val = val * (seq_t)10 + (seq_t)(start_digit - i);
    }

    return val;
}
