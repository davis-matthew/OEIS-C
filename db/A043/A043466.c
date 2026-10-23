/*
 * A043466  Numbers having two 3's in base 9.
 * https://oeis.org/A043466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑9 representation contains
 *            exactly two digits 3
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t size = 0;
    static size_t cap  = 0;
    static seq_t prev_val = 0;          /* last integer checked */
    static idx_t  current_count = 0;   /* how many terms have been stored */

    /* Allocate storage on first call. */
    if (seq == NULL) {
        cap = 64;
        seq = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (current_count < n) {
        prev_val++;            /* next candidate integer */
        seq_t  x   = prev_val;
        int    cnt = 0;

        /* Count how many base‑9 digits are equal to 3. */
        while (x != 0) {
            if ((x % 9) == 3) {
                ++cnt;
                if (cnt > 2) break;   /* early exit */
            }
            x /= 9;
        }

        if (cnt == 2) {        /* exactly two 3's ? */
            if (size == cap) { /* resize if needed */
                cap <<= 1;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
            }
            seq[size++] = prev_val;
            ++current_count;
        }
    }

    return seq[n - 1];
}
