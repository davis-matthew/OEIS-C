/*
 * A060301  For n>3: a(n) is a multiple of three distinct earlier terms.
 * https://oeis.org/A060301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3, a(3)=5; for n>3, a(n) is the sorted list of
 *            30·6^i·10^j·15^k (i,j,k ≥ 0). This follows the “Ugly‑numbers”
 *            algorithm with generators 6, 10 and 15
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static storage for the sequence */
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static size_t i6 = 0, i10 = 0, i15 = 0;
    static bool initialized = false;

    if (!initialized) {
        /* initialise with the first four terms: 2, 3, 5, 30 */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)5;
        seq[3] = (seq_t)30;
        seq_len = 4;
        i6 = i10 = i15 = 3;   /* indices that point to the term 30 */
        initialized = true;
    }

    /* Return the value if we already have enough terms */
    if (n <= seq_len)
        return seq[n - 1];

    /* Generate terms until we reach the desired index */
    while (seq_len < n) {
        seq_t next6 = seq[i6] * (seq_t)6;
        seq_t next10 = seq[i10] * (seq_t)10;
        seq_t next15 = seq[i15] * (seq_t)15;

        /* choose the smallest candidate */
        seq_t next_val = next6;
        if (next10 < next_val) next_val = next10;
        if (next15 < next_val) next_val = next15;

        /* append the new term, expanding the array if necessary */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = next_val;

        /* increment the indices that produced the chosen value */
        if (next_val == next6) ++i6;
        if (next_val == next10) ++i10;
        if (next_val == next15) ++i15;
    }

    return seq[n - 1];
}
