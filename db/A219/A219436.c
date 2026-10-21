/*
 * A219436  Unmatched value maps: number of nX3 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal,
 *          diagonal or antidiagonal neighbor in a random 0..1 nX3 array.
 * https://oeis.org/A219436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+a(n-2)+a(n-3)-2*a(n-4)-6*a(n-5)-3*a(n-6)-2*a(n-7)+5*a(n-8)+4*a(n-9)-2*a(n-10)-2*a(n-11);
 *            Initial values: a(1)=4, a(2)=18, a(3)=78, a(4)=329, a(5)=1396,
 *            a(6)=5925, a(7)=25147, a(8)=106731, a(9)=452984, a(10)=1922547,
 *            a(11)=8159665
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised terms */
    static seq_t *cache = NULL;   /* array indexed from 0..max_computed */
    static size_t cap   = 0;      /* allocated capacity */
    static idx_t  max_computed = 0;   /* largest index with a value stored */

    /* n must be >= 1; for n==0 return 0 as a safe default */
    if (n == 0) return 0;

    /* ensure capacity for indices up to n (and at least 12 for the base constants) */
    size_t needed_cap = (size_t)n + 1;
    if (needed_cap < 12) needed_cap = 12;
    if (cap < needed_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – abort gracefully */
            abort();
        }
        cache = tmp;
        cap   = needed_cap;
    }

    /* initialise the first 11 terms once, if not already done */
    if (max_computed < 11) {
        /* set index 0 to zero for safety */
        cache[0] = (seq_t)0;

        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)18;
        cache[3]  = (seq_t)78;
        cache[4]  = (seq_t)329;
        cache[5]  = (seq_t)1396;
        cache[6]  = (seq_t)5925;
        cache[7]  = (seq_t)25147;
        cache[8]  = (seq_t)106731;
        cache[9]  = (seq_t)452984;
        cache[10] = (seq_t)1922547;
        cache[11] = (seq_t)8159665;

        max_computed = 11;
    }

    /* if the requested term is already stored, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* compute a(i) from a(i-1)..a(i-11) */
        seq_t val = (seq_t)0;
        val += (seq_t)4 * cache[i-1];
        val += cache[i-2];
        val += cache[i-3];
        val -= (seq_t)2 * cache[i-4];
        val -= (seq_t)6 * cache[i-5];
        val -= (seq_t)3 * cache[i-6];
        val -= (seq_t)2 * cache[i-7];
        val += (seq_t)5 * cache[i-8];
        val += (seq_t)4 * cache[i-9];
        val -= (seq_t)2 * cache[i-10];
        val -= (seq_t)2 * cache[i-11];
        cache[i] = val;
    }

    max_computed = n;
    return cache[n];
}
