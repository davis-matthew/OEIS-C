/*
 * A052889  Number of rooted set partitions.
 * https://oeis.org/A052889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*Bell(n-1) (Bell(0)=1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for Bell numbers and the current Bell‑triangle row. */
    static seq_t *bell   = NULL;    /* bell[k] = Bell(k)      */
    static size_t bell_cap  = 0;    /* capacity of bell       */
    static size_t bell_last = 0;    /* largest n with Bell(n) computed */
    static seq_t *row    = NULL;    /* current row of the Bell triangle   */
    static size_t row_len   = 0;    /* length of current row  */

    /* Helper to initialise static arrays on first use. */
    if (bell == NULL) {
        bell_cap = 1;
        bell = (seq_t *)malloc(bell_cap * sizeof(seq_t));
        bell[0] = (seq_t)1;            /* Bell(0) = 1 */
        bell_last = 0;

        row_len = 1;
        row = (seq_t *)malloc(row_len * sizeof(seq_t));
        row[0] = (seq_t)1;              /* row[0] = 1  */
    }

    /* a(0) is defined as 0. */
    if (n == 0) return (seq_t)0;

    /* Ensure Bell(n-1) is available. */
    idx_t up_to = n - 1;
    if (bell_last < (size_t)up_to) {
        for (size_t i = bell_last + 1; i <= (size_t)up_to; ++i) {
            /* New row length is one larger. */
            size_t new_len = row_len + 1;
            seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));

            /* First element of new row = last element of previous row. */
            new_row[0] = row[row_len - 1];

            /* Remaining elements derived from previous row. */
            for (size_t j = 1; j < new_len; ++j) {
                new_row[j] = new_row[j - 1] + row[j - 1];
            }

            /* Store the new Bell number (first element of the new row). */
            if (i >= bell_cap) {
                size_t new_cap = bell_cap * 2;
                seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* In a real application, handle out‑of‑memory here. */
                    exit(1);
                }
                bell = tmp;
                bell_cap = new_cap;
            }
            bell[i] = new_row[0];

            /* Advance to next row. */
            free(row);
            row = new_row;
            row_len = new_len;
            bell_last = i;
        }
    }

    /* Compute the term using the formula a(n) = n * Bell(n‑1). */
    return ((seq_t)n) * bell[(size_t)(n - 1)];
}
