/*
 * A041129  Denominators of continued fraction convergents to sqrt(73).
 * https://oeis.org/A041129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2136*a(n-7) + a(n-14) (for n ≥ 14) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=11, a(4)=57, a(5)=68, a(6)=125,
 *            a(7)=2068, a(8)=2193, a(9)=4261, a(10)=23498, a(11)=121751,
 *            a(12)=145249, a(13)=267000
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[14] = {
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)11,  (seq_t)57,
        (seq_t)68, (seq_t)125, (seq_t)2068, (seq_t)2193, (seq_t)4261,
        (seq_t)23498, (seq_t)121751, (seq_t)145249, (seq_t)267000
    };

    /* If n is within the initially known range, return it directly. */
    if (n < 14U) {
        return init[(idx_t)n];
    }

    /* Allocate space for the sequence up to n. */
    idx_t len = (idx_t)n + 1U;
    seq_t *seq = (seq_t *)malloc(len * sizeof(seq_t));
    if (!seq) {
        /* In a real implementation we would handle allocation failure. */
        return (seq_t)0;
    }

    /* Copy the known initial values. */
    for (idx_t i = 0; i < 14U; ++i) {
        seq[i] = init[i];
    }

    /* Compute terms from 14 up to n using the recurrence. */
    const seq_t mult = (seq_t)2136U;
    for (idx_t i = 14U; i < len; ++i) {
        seq[i] = mult * seq[i - 7U] + seq[i - 14U];
    }

    /* Fetch the requested value and free temporary storage. */
    seq_t result = seq[n];
    free(seq);
    return result;
}
