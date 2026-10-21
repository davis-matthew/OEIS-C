/*
 * A205390  s(k)-s(j), where (k,j) is the least pair for which n divides
 *          s(k)-s(j), and s(j)=(1/2)C(2j,j).
 * https://oeis.org/A205390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the first positive difference s(k)-s(j)
   with n | (s(k)-s(j)) where s(m) = (1/2)·C(2m,m).
   The algorithm scans the values s(m) in order, keeps a
   table of the first index at which each residue modulo n
   occurs, and returns the difference when a residue repeats
   for the first time. */
#include <stdlib.h>

/* idx_t and seq_t are provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, return 0 */

    static seq_t *s_cache = NULL;    /* cached s(m) values, 1‑based */
    static idx_t s_cap  = 0;          /* current capacity of s_cache */

    /* ensure s_cache has values for m = 1 and 2 at start */
    if (s_cap < 2) {
        s_cache = (seq_t *)realloc(s_cache, sizeof(seq_t) * 2);
        if (!s_cache) return 0;
        s_cache[0] = 0;              /* unused */
        s_cache[1] = (seq_t)1;       /* s(1) = 1 */
        s_cap   = 2;
    }

    /* array that records the first occurrence of each residue modulo n */
    int *first = (int *)calloc((size_t)n, sizeof(int));
    if (!first) return 0;             /* allocation failed */

    idx_t j = 1;                      /* current index in s_cache */

    while (1) {
        /* residue of s(j) modulo n */
        seq_t rem = s_cache[j] % (seq_t)n;
        int r = (int)rem;             /* n <= 10000, so this fits in int */

        /* if this residue has seen before, return the difference */
        int pos = first[r];
        if (pos != 0) {
            seq_t diff = s_cache[j] - s_cache[pos];
            free(first);
            return diff;
        }
        first[r] = (int)j;            /* remember first occurrence */

        /* compute s(j+1) using the recurrence:
           s(m) = s(m-1) * (4*m - 2) / m  */
        idx_t next = j + 1;
        if (next >= s_cap) {
            size_t new_cap = s_cap * 2;
            if (new_cap <= (size_t)next) new_cap = (size_t)next + 1;
            seq_t *tmp = (seq_t *)realloc(s_cache, sizeof(seq_t) * new_cap);
            if (!tmp) {
                free(first);
                return 0;
            }
            s_cache = tmp;
            s_cap   = new_cap;
        }
        seq_t prev = s_cache[j];
        seq_t mult = prev * (seq_t)(4 * next - 2);
        s_cache[next] = mult / (seq_t)next;

        j = next;
    }
}
