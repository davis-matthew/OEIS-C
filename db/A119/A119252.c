/*
 * A119252  Odd digits in decreasing order.
 * https://oeis.org/A119252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 30, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A119252:  Odd digits in decreasing order
 *
 *  The terms are all numbers whose decimal digits are odd (1,3,5,7,9)
 *  and appear in decreasing order from left to right.  The list is
 *  finite: there are 2^5−1 = 31 such non‑empty sequences.  The values
 *  are generated once and cached.
 *
 *  The function returns a(1)…a(31).  For n outside this range it
 *  returns 0.  The implementation works for any seq_t type supplied
 *  by the build (uint8_t … unsigned __int128, arbitrary‑precision, …).
 */
#include <stdlib.h>

static int cmp_seq_t(const void *va, const void *vb)
{
    const seq_t *a = (const seq_t *)va;
    const seq_t *b = (const seq_t *)vb;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /*  There are exactly 31 such numbers.  */
    static seq_t values[31];
    static int initialized = 0;

    if (!initialized) {
        const int digits[5] = {1, 3, 5, 7, 9};
        int count = 0;

        for (int mask = 1; mask < (1 << 5); ++mask) {
            seq_t num = 0;
            /* Build the number from most significant to least digit. */
            for (int bit = 4; bit >= 0; --bit) {
                if (mask & (1 << bit)) {
                    num = num * 10 + (seq_t)digits[bit];
                }
            }
            values[count++] = num;
        }

        /* The generated numbers are already unique; sort them ascending. */
        if (count == 31) {
            qsort(values, 31, sizeof(seq_t), cmp_seq_t);
        }
        initialized = 1;
    }

    if (n == 0 || n > 31) {
        return 0;
    }
    return values[n - 1];
}
