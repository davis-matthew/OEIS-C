/*
 * A205788  Least positive integer j such that n divides k^3-j^3, where k, as
 *          in A205787, is the least number for which there is such a j.
 * https://oeis.org/A205788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find for each n ≥ 1 the least k such that there exists j < k 
   with k³ ≡ j³ (mod n).  Then a(n) is that j.  The algorithm uses an
   array of size n storing, for each residue modulo n, the smallest j seen;
   as soon as a duplicate residue appears we return the stored j.  */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0 (guard) */

    /* size of the residue table */
    size_t len = (size_t)n;
    idx_t *first = (idx_t *)malloc(len * sizeof(idx_t));
    if (!first) return 0;   /* memory allocation failure */

    for (size_t i = 0; i < len; ++i) first[i] = 0;

    seq_t n_seq = (seq_t)n;
    /* handle k = 1 first */
    seq_t k_seq = 1;
    seq_t km = k_seq % n_seq;
    seq_t r = ((km * km) % n_seq * km) % n_seq;   /* 1³ mod n */
    first[(size_t)r] = 1;                         /* j = 1 stores residue */

    /* search k from 2 up to n+1 (pigeonhole guarantees a match) */
    for (idx_t k = 2; k <= n + 1; ++k) {
        k_seq = k;
        km = k_seq % n_seq;
        r = ((km * km) % n_seq * km) % n_seq;     /* k³ mod n */
        idx_t j = first[(size_t)r];
        if (j != 0) {
            free(first);
            return (seq_t)j;                     /* found minimal j */
        }
        first[(size_t)r] = k;                    /* store first j for residue */
    }

    free(first);
    return 0;   /* should never reach here */
}
