/*
 * A088131  a(n) equals the square of the n-th partial sum added to twice the
 *          n-th partial sum of the squares, divided by a(n-1), for all n>=1,
 *          with a(0)=1, a(1)=1.
 * https://oeis.org/A088131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-3) for n>3; a(0)=1, a(1)=1, a(2)=8,
 *            a(3)=29;
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;
static idx_t computed = 0;

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)29;

    /* ensure storage is large enough */
    if (computed <= n) {
        idx_t new_size = n + 1;
        if (seq == NULL) {
            seq = (seq_t*)malloc(new_size * sizeof(seq_t));
            if (!seq) return (seq_t)0;          /* allocation failure */
            seq[0] = (seq_t)1;
            seq[1] = (seq_t)1;
            seq[2] = (seq_t)8;
            seq[3] = (seq_t)29;
            computed = 4;
        } else {
            seq = (seq_t*)realloc(seq, new_size * sizeof(seq_t));
            if (!seq) return (seq_t)0;          /* allocation failure */
        }
        /* compute new entries */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t temp = seq[i-1];
            temp = temp * (seq_t)4;
            seq[i] = temp - seq[i-3];
        }
        computed = n + 1;
    }

    return seq[n];
}
