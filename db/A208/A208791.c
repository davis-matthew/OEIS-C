/*
 * A208791  Number of nX3 0..2 arrays with no element equal the average of
 *          immediate neighbors vertically above, diagonally above and left,
 *          and horizontally left of it.
 * https://oeis.org/A208791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=12, a(2)=160, a(3)=2284, a(4)=32446, a(5)=461280,
 *            a(6)=6557958, a(7)=93227592, a(8)=1325375622, a(9)=18841850112;
 *            For n≥10:
 *            a(n)=4*a(n−1)+123*a(n−2)+332*a(n−3)−170*a(n−4)−796*a(n−5)
 *            +124*a(n−6)+464*a(n−7)−88*a(n−8)−16*a(n−9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *seq = NULL;
    static idx_t size = 0;
    static idx_t last = 0;          /* largest index that has a value */
    static int initialized = 0;     /* have we stored the first nine terms? */

    /* at least enough space for the nine base terms (indices 1..9) */
    if (size < 10) {
        seq_t *tmp = (seq_t *)realloc(seq, 10 * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        seq = tmp;
        size = 10;
    }

    /* initialise the first nine terms once */
    if (!initialized) {
        seq[1] = 12;
        seq[2] = 160;
        seq[3] = 2284;
        seq[4] = 32446;
        seq[5] = 461280;
        seq[6] = 6557958;
        seq[7] = 93227592;
        seq[8] = 1325375622;
        seq[9] = 18841850112;
        last = 9;
        initialized = 1;
    }

    /* tiny optimisation: if caller requests an index we already know */
    if (n <= last) return seq[n];

    /* grow storage if necessary */
    if (n + 1 > size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_size * sizeof(seq_t));
        if (!tmp) return 0;
        seq = tmp;
        size = new_size;
    }

    /* compute missing values with the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq[i] = (seq_t)4 * seq[i - 1]
               + (seq_t)123 * seq[i - 2]
               + (seq_t)332 * seq[i - 3]
               - (seq_t)170 * seq[i - 4]
               - (seq_t)796 * seq[i - 5]
               + (seq_t)124 * seq[i - 6]
               + (seq_t)464 * seq[i - 7]
               - (seq_t)88  * seq[i - 8]
               - (seq_t)16  * seq[i - 9];
    }
    last = n;

    return seq[n];
}
