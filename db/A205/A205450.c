/*
 * A205450  Least k such that n divides s(k) - s(j) for some j<k, where s(j)
 *          is the 2j-th Fibonacci number.
 * https://oeis.org/A205450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides F_{2k} - F_{2j} for some j<k,
 *            where F_m is the m‑th Fibonacci number (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined case – return 0 */

    /* Allocate an array that stores the first index where each residue occurs.
       For n up to about 2 million this allocation is trivial (≈8 MB for 64‑bit). */
    size_t N = (size_t)n;
    idx_t *first = (idx_t*)malloc(N * sizeof(idx_t));
    if (!first) return (seq_t)-1;          /* allocation failed (unlikely) */

    for (size_t i = 0; i < N; ++i) first[i] = (idx_t)-1;

    seq_t modulus = (seq_t)n;
    seq_t f_prev = 0;                      /* F_0 */
    seq_t f_cur  = 1 % modulus;            /* F_1 modulo n */
    size_t  fib_index = 1;                 /* current Fibonacci index i */

    while (1) {
        /* Compute next Fibonacci number modulo n. */
        seq_t next = (f_prev + f_cur) % modulus;
        f_prev = f_cur;
        f_cur  = next;
        fib_index++;                       /* now at F_{fib_index} */

        if ((fib_index & 1) == 0) {        /* even index => a term of the t_k sequence */
            idx_t k = fib_index >> 1;      /* k = fib_index / 2 */
            size_t rindex = (size_t)f_cur; /* the residue is < n, safe to cast to size_t */

            if (first[rindex] == (idx_t)-1) {
                first[rindex] = k;        /* first occurrence of this residue */
            } else {
                seq_t result = (seq_t)k;   /* first repeat found */
                free(first);
                return result;
            }
        }
    }
}
