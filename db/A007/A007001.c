/*
 * A007001  Trajectory of 1 under the morphism 1 -> 12, 2 -> 123, 3 -> 1234,
 *          etc.
 * https://oeis.org/A007001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   fixed point of the morphism k -> 1,2,...,k+1 starting from 1;
 *            built self-generatingly: for i = 1, 2, ..., append the block
 *            1,2,...,a(i)+1
 * Range:     u8 n <= 4862, u16 n <= 4862, u32 n <= 4862, u64 n <= 4862,
 *            u128 n <= 4862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static unsigned int *s = NULL;
    static idx_t len = 0;   /* number of generated terms (positions 1..len) */
    static idx_t cap = 0;
    static idx_t next = 1;  /* next index whose block is to be appended */

    if (s == NULL) {
        cap = 64;
        s = (unsigned int *)malloc((size_t)(cap + 1) * sizeof(unsigned int));
        s[1] = 1;
        len = 0;
        next = 1;
    }

    while (len < n) {
        unsigned int k = s[next];
        idx_t need = len + (idx_t)k + 1;
        if (need > cap) {
            idx_t nc = cap * 2;
            while (nc < need) nc *= 2;
            s = (unsigned int *)realloc(s, (size_t)(nc + 1) * sizeof(unsigned int));
            cap = nc;
        }
        for (unsigned int j = 1; j <= k + 1; j++) {
            s[++len] = j;
        }
        next++;
    }

    return (seq_t)s[n];
}
