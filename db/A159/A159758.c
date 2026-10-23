/*
 * A159758  Positive numbers y such that y^2 is of the form x^2+(x+79)^2 with
 *          integer x.
 * https://oeis.org/A159758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) (n>6), a(1)=65, a(2)=79, a(3)=101,
 *            a(4)=289, a(5)=395, a(6)=541
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 33, u64 n <= 69, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static idx_t size = 0;

    if (n == 0) return 0; /* unused, but safe */

    if (n > size) {
        idx_t new_size = n;
        seq = (seq_t*)realloc(seq, new_size * sizeof(seq_t));
        if (!seq) return 0; /* allocation failure */

        for (idx_t i = size + 1; i <= new_size; ++i) {
            seq_t val;
            if (i == 1) val = 65;
            else if (i == 2) val = 79;
            else if (i == 3) val = 101;
            else if (i == 4) val = 289;
            else if (i == 5) val = 395;
            else if (i == 6) val = 541;
            else val = 6 * seq[i - 4] - seq[i - 7]; /* i-3-1 = i-4, i-6-1 = i-7 */
            seq[i - 1] = val;
        }
        size = new_size;
    }
    return seq[n - 1];
}
