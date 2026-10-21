/*
 * A202796  Number of n X 2 binary arrays with every one adjacent to another
 *          one horizontally or vertically.
 * https://oeis.org/A202796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 4*a(n-3) initial values: a(1)=2,
 *            a(2)=10, a(3)=36
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple memoisation table that grows on demand.
   In C++ a C++ array with proper constructors/destructors is used;
   in C the table is a malloc/realloc buffer.  The code remains
   valid C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range – not used */

    /* static state */
    static seq_t *mem = NULL;   /* array of computed values 1..last */
    static idx_t last = 0;      /* highest n for which mem[n-1] is valid */
    static size_t cap  = 0;     /* allocated capacity */

    /* grow the table if needed */
    if ((size_t)n > cap) {
#ifdef __cplusplus
        /* allocate a new array with C++ constructor support */
        size_t new_cap = (size_t)n;
        seq_t *tmp = new seq_t[new_cap];
        if (mem) {
            for (size_t i = 0; i < cap; ++i)
                tmp[i] = mem[i];
            delete[] mem;
        }
        mem = tmp;
        cap = new_cap;
#else
        /* C: reallocate with malloc/realloc */
        size_t new_cap = (size_t)n;
        seq_t *tmp = realloc(mem, new_cap * sizeof *mem);
        if (!tmp) abort();           /* allocation failed */
        mem = tmp;
        cap = new_cap;
#endif
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            mem[0] = (seq_t)2;
        } else if (i == 2) {
            mem[1] = (seq_t)10;
        } else if (i == 3) {
            mem[2] = (seq_t)36;
        } else {
            seq_t v1 = mem[i-2];   /* a(i-1) */
            seq_t v2 = mem[i-3];   /* a(i-2) */
            seq_t v3 = mem[i-4];   /* a(i-3) */
            mem[i-1] = (seq_t)(3*v1 + v2 + 4*v3);
        }
    }
    last = n;
    return mem[n-1];
}
