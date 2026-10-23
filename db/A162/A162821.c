/*
 * A162821  Positive numbers k such that 30*k/(30-k) are integers.
 * https://oeis.org/A162821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive numbers k such that 30*k/(30-k) is an integer.
   This is equivalent to |30-k| dividing 900, with k=30 explicitly
   included.  The sequence is finite (41 terms, last being 930). */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Divisors of 900 that are < 30 (used for k < 30). */
    const idx_t small[] = {1, 2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 25};
    const idx_t count_small = sizeof(small) / sizeof(small[0]);   /* 13 */

    /* All positive divisors of 900, in ascending order. */
    const idx_t all[] = {1, 2, 3, 4, 5, 6, 9, 10, 12, 15, 18, 20, 25,
                         30, 36, 45, 50, 60, 75, 90, 100, 150, 180,
                         225, 300, 450, 900};
    const idx_t count_all = sizeof(all) / sizeof(all[0]);         /* 27 */

    /* Lower part of the sequence (k < 30). */
    if (n <= count_small) {
        /* n=1 → k=5,   n=13 → k=29 */
        return (seq_t)(30 - small[count_small - n]);
    }

    /* The special term k = 30. */
    if (n == count_small + 1) {
        return (seq_t)30;
    }

    /* Upper part of the sequence (k > 30). */
    if (n > count_small + 1 + count_all) {
        /* Out of bounds – return 0. */
        return (seq_t)0;
    }

    /* n = 15 corresponds to 30 + 1, n = 41 corresponds to 30 + 900. */
    idx_t idx = n - count_small - 2;          /* 0‑based index into 'all' */
    return (seq_t)(30 + all[idx]);
}
