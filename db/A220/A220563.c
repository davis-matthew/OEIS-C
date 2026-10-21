/*
 * A220563  Number of ways to reciprocally link elements of an 2 X n array
 *          either to themselves or to exactly one horizontal or antidiagonal
 *          neighbor.
 * https://oeis.org/A220563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - a(n-4) (n ≥ 5) Initial values:
 *            a(1)=1, a(2)=5, a(3)=14, a(4)=47
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* storage for computed terms */
    static size_t sz   = 0;     /* current buffer size    */
    static size_t computed = 0; /* highest index already computed */

    /* For safety – sequence is defined for n ≥ 1 */
    if (n == 0) return (seq_t)0;

    size_t need = (size_t)n;

    /* Resize the buffer if required */
    if (need >= sz) {
        size_t new_sz = need + 1;                     /* indices 0…n */
        seq_t *tmp = (seq_t*)realloc(buf, new_sz * sizeof(seq_t));
        if (!tmp) {                                   /* allocation failed */
            free(buf);
            buf = NULL;
            sz = 0;
            computed = 0;
            return (seq_t)0;
        }
        buf = tmp;
        sz = new_sz;

        /* Initialise the first four terms if they are not already present */
        if (sz > 1) buf[1] = (seq_t)1;
        if (sz > 2) buf[2] = (seq_t)5;
        if (sz > 3) buf[3] = (seq_t)14;
        if (sz > 4) buf[4] = (seq_t)47;

        /* Mark the base terms as computed */
        if (computed < 4) computed = 4;
    }

    /* If the requested index is already computed, return it */
    if (n <= computed) return buf[n];

    /* Compute missing terms up to n */
    for (size_t i = computed + 1; i <= need; ++i) {
        buf[i] = buf[i-1] * (seq_t)2 + buf[i-2] * (seq_t)4 - buf[i-4];
    }
    computed = need;

    return buf[n];
}
