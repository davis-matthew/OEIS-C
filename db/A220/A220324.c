/*
 * A220324  Equals two maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements equal to exactly two of their
 *          horizontal and antidiagonal neighbors in a random 0..1 n X 3
 *          array.
 * https://oeis.org/A220324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=16, a(3)=92, a(4)=508,
 *            a(n)=7*a(n-1)-9*a(n-2)+3*a(n-3)+6*a(n-4) for n≥5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    if (a_cache_size < n) {
        size_t new_size = (size_t)n + 1;          /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        a_cache = tmp;

        for (idx_t i = a_cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                a_cache[i] = (seq_t)2;
            } else if (i == 2) {
                a_cache[i] = (seq_t)16;
            } else if (i == 3) {
                a_cache[i] = (seq_t)92;
            } else if (i == 4) {
                a_cache[i] = (seq_t)508;
            } else {
                seq_t t1 = (seq_t)7 * a_cache[i-1];
                seq_t t2 = (seq_t)3 * a_cache[i-3];
                seq_t t3 = (seq_t)6 * a_cache[i-4];
                seq_t sum = t1 + t2 + t3;
                seq_t sub = (seq_t)9 * a_cache[i-2];
                a_cache[i] = sum - sub;
            }
        }
        a_cache_size = n;
    }
    return a_cache[n];
}
