/*
 * A054709  Number of powers of 8 modulo n.
 * https://oeis.org/A054709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of distinct residues of powers of 8 modulo n
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8640 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count the distinct residues of 8^k (k ≥ 0) modulo n. */
static seq_t count_powers_of_8(idx_t n)
{
    /* Allocate a visited flag array; initialized to 0 by calloc. */
    unsigned char *visited = (unsigned char *)calloc((size_t)n, 1);
    if (!visited)                    /* allocation failure (unlikely for n up to 10000) */
        return 0;

    seq_t count = 0;                  /* number of distinct residues found */
    idx_t x = 1 % n;                  /* 8^0 mod n */

    /* Iterate until a residue repeats. */
    while (!visited[x]) {
        visited[x] = 1;
        ++count;
        seq_t t = ((seq_t)x * (seq_t)8) % (seq_t)n;
        x = (idx_t)t;
    }

    free(visited);
    return count;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined, but guard for completeness */
        return 0;

    return count_powers_of_8(n);
}
