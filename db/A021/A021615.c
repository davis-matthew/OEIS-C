/*
 * A021615  Decimal expansion of 1/611.
 * https://oeis.org/A021615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/611) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *digits = NULL;          /* cached digits */
    static size_t max_cached = 0;         /* number of digits cached */
    static unsigned long long rem = 1;    /* remainder after last digit */

    size_t idx = (size_t)n;
    if (idx >= max_cached) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t*)realloc(digits, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* allocation failure – return 0 */
        digits = tmp;
        for (size_t i = max_cached; i <= idx; ++i) {
            rem *= 10ULL;
            digits[i] = (seq_t)(rem / 611ULL);
            rem %= 611ULL;
        }
        max_cached = new_len;
    }
    return digits[idx];
}
