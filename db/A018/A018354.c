/*
 * A018354  Divisors of 246.
 * https://oeis.org/A018354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 246; found by enumerating all divisors of 246. */

#include <stddef.h>
#include <stdbool.h>

/* Compute all divisors of the fixed number 246 and store them in
   the supplied array.  The number of divisors is returned in `cnt`.  */
static void compute_divisors(seq_t *out, size_t *cnt)
{
    const seq_t target = (seq_t)246;
    size_t i, j = 0;
    for (idx_t d = 1; (idx_t)d * (idx_t)d <= (idx_t)target; ++d) {
        if (target % (seq_t)d == 0) {
            out[j++] = (seq_t)d;
            seq_t other = target / (seq_t)d;
            if (other != (seq_t)d) out[j++] = other;
        }
    }
    /* Sort the divisors in ascending order (simple insertion sort). */
    for (i = 1; i < j; ++i) {
        seq_t key = out[i];
        size_t k = i;
        while (k > 0 && out[k-1] > key) {
            out[k] = out[k-1];
            --k;
        }
        out[k] = key;
    }
    *cnt = j;
}

/* Return the n-th divisor of 246 (1‑based).  The sequence is finite
   (8 terms).  For n outside this range the function returns 0. */
seq_t a(idx_t n)
{
    static seq_t divisors[16];
    static size_t divcnt = 0;
    static bool initialized = false;
    if (!initialized) {
        compute_divisors(divisors, &divcnt);
        initialized = true;
    }
    if (n == 0 || n > divcnt) return (seq_t)0;
    return divisors[(size_t)n - 1];
}
