/*
 * A018537  Divisors of 594.
 * https://oeis.org/A018537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 594 (a(1)=1, a(16)=594).
   The function enumerates all divisors of 594, sorts them in
   ascending order, and returns the n‑th one for n >= 1.
   For indices beyond the 16th, a zero value is returned. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Static storage for the 16 divisors of 594. */
    static seq_t divs[16];
    static size_t dcnt = 0;

    if (dcnt == 0) {
        const seq_t limit = (seq_t)594;   /* 594 = 2 * 3^3 * 11 */
        /* Generate all divisors (unordered). */
        size_t count = 0;
        for (idx_t i = 1; (seq_t)i <= limit; ++i) {
            if (limit % (seq_t)i == 0) {
                divs[count++] = (seq_t)i;
            }
        }
        dcnt = count;   /* Should be 16 for 594. */

        /* Simple insertion sort on the array of divisors. */
        for (size_t i = 1; i < dcnt; ++i) {
            seq_t key = divs[i];
            idx_t j = (idx_t)(i - 1);
            while (j >= 0 && divs[j] > key) {
                divs[j + 1] = divs[j];
                --j;
            }
            divs[j + 1] = key;
        }
    }

    if (n >= 1 && (idx_t)n <= dcnt)
        return divs[(size_t)(n - 1)];
    /* Undefined indices: return 0 as a safe fallback. */
    return (seq_t)0;
}
