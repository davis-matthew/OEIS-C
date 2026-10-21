/*
 * A205115  The index j<k such that n divides s(k)-s(j), where k is the least
 *          index (A205114) for which such j exists, and s=(1,3,4,7,11,18...),
 *          the Lucas numbers.
 * https://oeis.org/A205115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute a(n) as the index j of the first repeated Lucas residue
 *            modulo n (i.e., the smallest j<k such that n | (s(k)-s(j)),
 *            where s = (1,3,4,7,11,18,…) are the Lucas numbers)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return 0;

    /* Allocate table of first occurrences, initially set to UINT64_MAX */
    idx_t *first = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    if (!first) return 0;  /* allocation failure */
    memset(first, 0xFF, (size_t)n * sizeof(idx_t));

    /* Lucas numbers modulo n */
    idx_t prev = 1 % n;                /* s(1) */
    first[prev] = 1;
    if (first[prev] != 1) {           /* already seen (only possible for n==1) */
        seq_t ans = (seq_t)first[prev];
        free(first);
        return ans;
    }

    idx_t curr = 3 % n;                /* s(2) */
    if (first[curr] == (idx_t)-1) {
        first[curr] = 2;
    } else {
        seq_t ans = (seq_t)first[curr];
        free(first);
        return ans;
    }

    idx_t k = 3;
    for (;;) {
        /* Compute next Lucas number modulo n: (prev + curr) % n */
        idx_t next = (idx_t)(((__uint128_t)prev + curr) % n);
        prev = curr;
        curr = next;

        if (first[next] == (idx_t)-1) {
            first[next] = k;
        } else {
            seq_t ans = (seq_t)first[next];
            free(first);
            return ans;
        }
        k++;
    }
}
