/*
 * A115589  Multiply first digit by k, append result to sequence; multiply
 *          second digit by k, append result to sequence; multiply third digit
 *          by k, append result to sequence; etc. a(1)=1, k=7 case.
 * https://oeis.org/A115589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generation: start with 1, then for each term read its decimal digits
   from most significant to least, multiply each by 7, append the results. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* undefined; offset 1 */
    }

    /* Allocate storage for up to n terms; all terms are ≤ 63. */
    seq_t *seq = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!seq) {
        return 0;                 /* allocation failure (should not happen) */
    }

    seq[0] = 1;
    size_t len = 1;               /* number of terms already stored */
    size_t idx = 0;               /* index of the term being processed */
    const seq_t k = 7;            /* multiplier */

    while (len < n) {
        seq_t val = seq[idx];

        /* Extract digits of val (at most 2 digits for this sequence). */
        uint8_t digits[2];
        size_t dlen = 0;
        seq_t temp = val;
        while (temp > 0 && dlen < 2) {
            digits[dlen++] = (uint8_t)(temp % 10);
            temp /= 10;
        }

        /* Process digits from most significant to least. */
        for (size_t d = 0; d < dlen && len < n; ++d) {
            size_t pos = dlen - 1 - d;          /* reverse order */
            seq[len++] = digits[pos] * k;
        }

        ++idx;
    }

    seq_t result = seq[n - 1];
    free(seq);
    return result;
}
