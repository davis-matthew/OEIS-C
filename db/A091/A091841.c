/*
 * A091841  Records in A091840.
 * https://oeis.org/A091841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1 For n >= 1: if n even: a(n) = 4*a(n-1) - (3*n - 3) else
 *            if n mod 4==1: a(n) = 4*a(n-1) - (3*n - 2) else (n mod 4==3):
 *            a(n) = 4*a(n-1) - (3*n - 5) (Thus a(n) = 4*a(n-1) - d(n) where
 *            d(n) = 3*n - offset with offset depending on the congruence
 *            class of n modulo 4.)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --------- static memoisation --------- */
    static seq_t *mem = NULL;      /* array of computed terms */
    static idx_t mem_used = 0;     /* number of filled entries */
    static idx_t mem_cap  = 0;     /* current allocated size      */

    /* --- initialise memoisation array if needed --- */
    if (mem == NULL) {
        mem_cap  = 1;
        mem_used = 1;
        mem = (seq_t *)malloc(mem_cap * sizeof(seq_t));
        mem[0] = (seq_t)1;         /* a(0) = 1 */
    }

    /* --- extend memoisation array to cover requested n --- */
    if ((idx_t)n >= mem_cap) {
        /* grow capacity (doubling is cheap for n <= 10^4) */
        while (mem_cap <= n) mem_cap <<= 1;
        mem = (seq_t *)realloc(mem, mem_cap * sizeof(seq_t));
    }

    /* --- compute terms up to n if not already done --- */
    if ((idx_t)n >= mem_used) {
        for (idx_t i = mem_used; i <= n; ++i) {
            seq_t prev = mem[i - 1];

            /* offset = 3 for even i, 2 for i≡1 (mod 4), 5 for i≡3 (mod 4) */
            int offset;
            if ((i & 1ULL) == 0)      /* even */
                offset = 3;
            else if ((i & 3ULL) == 1) /* i % 4 == 1 */
                offset = 2;
            else                      /* i % 4 == 3 */
                offset = 5;

            /* d(i) = 3*i - offset */
            seq_t d = ((seq_t)3 * (seq_t)i) - (seq_t)offset;

            /* a(i) = 4*prev - d */
            mem[i] = (seq_t)4 * prev - d;
        }
        mem_used = n + 1;
    }

    return mem[n];
}
