/*
 * A048410  Numbers whose consecutive digits differ by 8.
 * https://oeis.org/A048410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048410: Numbers whose consecutive digits differ by 8.
 *
 * Generation in ascending numeric order:
 *   a(n) = n                     for 0 ≤ n ≤ 9          (single‑digit numbers)
 *   a(n) = number of length L ≥ 2 with alternating digits:
 *            start 1 (1,9,1,9,…),
 *            start 8 (8,0,8,0,…),
 *            start 9 (9,1,9,1,…)
 *   L and the pattern are determined by
 *        k   = n - 10         (k ≥ 0)
 *        L   = 2 + k / 3
 *        pos = k % 3          (0 → start 1, 1 → start 8, 2 → start 9)
 *
 * This function works for any unsigned integer type supplied as idx_t
 * and any unsigned integer type supplied as seq_t (8, 16, 32, 64, 128
 * bits or arbitrary‑precision).  It does not use any global state
 * and is O(length of output number), which is ≤ O(n).
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)n;

    /* index offset after the single‑digit terms */
    idx_t k   = n - 10;
    idx_t L   = 2 + k / 3;     /* length of the desired number (≥ 2) */
    int   pos = k % 3;         /* pattern selection */

    /* Determine the pair of alternating digits for the chosen pattern */
    int d1, d2;
    if (pos == 0) {        /* start 1 → 1,9,1,9,… */
        d1 = 1; d2 = 9;
    } else if (pos == 1) { /* start 8 → 8,0,8,0,… */
        d1 = 8; d2 = 0;
    } else {               /* pos == 2, start 9 → 9,1,9,1,… */
        d1 = 9; d2 = 1;
    }

    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        /* i is zero‑based; use d1 for the first digit, then d2 */
        int digit = (i & 1) ? d2 : d1;
        val = val * 10 + (seq_t)digit;
    }

    return val;
}
