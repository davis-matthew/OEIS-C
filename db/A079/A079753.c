/*
 * A079753  Operation count to create all permutations of n distinct elements
 *          using the "streamlined" version of Algorithm L (lexicographic
 *          permutation generation) from Knuth's The Art of Computer
 *          Programming, Vol. 4, chapter 7.2.1.2. Sequence gives total
 *          executions of step L3.1'.
 * https://oeis.org/A079753
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(3) = 0; for n>=4, a(n) = n*a(n-1) + (n-1)*(n-2)/2
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoization for any order of calls */
    static seq_t *mem = NULL;
    static idx_t last_n = 3;       /* highest n computed so far */
    static size_t mem_size = 0;    /* allocated size */

/* initialise storage once */
    if (mem == NULL) {
        mem_size = 4;                     /* indices 0..3 */
        mem = (seq_t *)calloc(mem_size, sizeof(seq_t));
        if (mem == NULL) return (seq_t)0; /* allocation failure */
        mem[3] = (seq_t)0;                /* a(3)=0 */
    }

/* Calls below offset are irrelevant – return 0 */
    if (n < 3)
        return (seq_t)0;

/* grow table if necessary */
    if ((size_t)(n + 1) > mem_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(mem, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failure */
        /* initialise new entries to 0 */
        for (size_t i = mem_size; i < new_size; ++i)
            tmp[i] = (seq_t)0;
        mem = tmp;
        mem_size = new_size;
    }

/* compute missing values up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)i * mem[i - 1];
        seq_t term2 = (seq_t)(i - 1) * (seq_t)(i - 2) / 2;
        mem[i] = term1 + term2;
    }
    last_n = n;

    return mem[n];
}
