/*
 * A119133  Numbers k such that the k-th triangular number contains only
 *          digits {1,5,6}.
 * https://oeis.org/A119133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 34, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the k-th triangular number contains only digits {1,5,6}.
   a(n) = the nth integer k for which T_k = k*(k+1)/2 consists exclusively of
   the digits 1, 5, and 6. */
#include <stdlib.h>
#include <stdbool.h>

#define SEQ_MAX 20000   /* large enough for n up to about 10000 */

static seq_t seq_k[SEQ_MAX];
static size_t seq_k_size = 0;
static idx_t next_k = 1;

/* Return the n-th element of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0 || n > SEQ_MAX)
        return 0;   /* out of bounds; sequence not defined for such n */

    while (seq_k_size < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t T = k * (k + 1) / 2;   /* triangular number */

        /* Check whether T contains only digits 1, 5, 6 */
        seq_t t = T;
        bool ok = true;
        while (t > 0) {
            uint8_t d = (uint8_t)(t % 10);
            if (d != 1 && d != 5 && d != 6) {
                ok = false;
                break;
            }
            t /= 10;
        }

        if (ok) {
            seq_k[seq_k_size++] = k;
        }
        next_k++;
    }

    return seq_k[(size_t)n - 1];
}
