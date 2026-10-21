/*
 * A236580  The number of tilings of a 6 X (4n) floor with 1 X 4 tetrominoes.
 * https://oeis.org/A236580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4)
   with initial values a(0)=1, a(1)=4, a(2)=25, a(3)=154 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *mem = NULL;
    static idx_t cached = 0;   /* highest index computed so far */

    /* Ensure we have at least the first four values */
    if (!mem) {
        size_t need = (size_t)(n + 1);
        if (need < 4) need = 4;          /* at least indices 0..3 */
        mem = (seq_t*)calloc(need, sizeof(seq_t));
        if (!mem) exit(1);
        mem[0] = (seq_t)1;
        mem[1] = (seq_t)4;
        mem[2] = (seq_t)25;
        mem[3] = (seq_t)154;
        cached = 3;
    }

    /* If the requested index is already computed, return it */
    if (n <= cached)
        return mem[n];

    /* Resize storage to hold up to n */
    size_t newsize = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(mem, newsize * sizeof(seq_t));
    if (!tmp) exit(1);
    mem = tmp;

    /* Compute any missing terms */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t t1 = mem[i-1] * (seq_t)7;          /* 7*a(n-1) */
        seq_t t2 = mem[i-3] * (seq_t)4;          /* 4*a(n-3) */
        seq_t sub = mem[i-2] * (seq_t)6;         /* 6*a(n-2) */
        sub += mem[i-4];                         /* + a(n-4) */
        mem[i] = t1 + t2 - sub;                  /* recurrence */
    }

    cached = n;
    return mem[n];
}
