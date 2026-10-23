/*
 * A135998  Smallest error in trying to solve n^3 = x^3 + y^3. That is, for
 *          each n, find positive integers x <= y < n such that | n^3 - x^3 -
 *          y^3 | is minimal and let a(n) := n^3 - x^3 - y^3.
 * https://oeis.org/A135998
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the minimal difference n^3 - x^3 - y^3 over 1 <= x <= y < n.
   If several pairs give the same absolute difference, the positive difference
   is chosen (ties with negative are discarded). */
#include <stdbool.h>
#include <stdint.h>

static inline seq_t cube(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * kk * kk;
}

seq_t a(idx_t n)
{
    /* offset 2: for n < 2 the value is not defined; return 0 */
    if (n < 2) return 0;

    seq_t target = cube(n);

    /* two‑pointer search over 1 <= x <= y < n */
    idx_t i = 1;
    idx_t j = n - 1;
    seq_t best_abs = target;        /* any difference cannot exceed target */
    seq_t best_diff = target;       /* placeholder */

    while (i <= j) {
        seq_t sum = cube(i) + cube(j);
        seq_t diff = target - sum;      /* may be negative */

        seq_t abs_diff = diff < 0 ? -diff : diff;
        if (abs_diff < best_abs ||
            (abs_diff == best_abs && diff > best_diff)) {
            best_abs = abs_diff;
            best_diff = diff;
        }

        /* Move pointers to explore sums closer to target */
        if (diff > 0) {
            /* sum < target, need larger sum → increase i */
            i++;
        } else if (diff < 0) {
            /* sum > target, need smaller sum → decrease j */
            j--;
        } else {
            /* exact match – cannot find better */
            break;
        }
    }

    return best_diff;
}
