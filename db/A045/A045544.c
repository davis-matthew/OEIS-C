/*
 * A045544  Odd values of n for which a regular n-gon can be constructed by
 *          compass and straightedge.
 * https://oeis.org/A045544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn, nice
 * Formula:   a(n) = product of distinct odd Fermat primes (non‑empty) sorted
 *            ascending. For n ≥ 1 : a(1)=3, a(2)=5, a(3)=15, …,
 *            a(31)=4294967295. If n > 31, a(n) = 0
 * Range:     u8 none, u16 none, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static table of all 31 possible products
   of the five known Fermat primes 3, 5, 17, 257 and 65537.
   The table is filled once and sorted using qsort. */

#include <stdlib.h>

/* Comparator that works for both C and C++ regardless of seq_t type. */
static int cmp_seq(const void *x, const void *y)
{
    const seq_t *a = (const seq_t *)x;
    const seq_t *b = (const seq_t *)y;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Return the n‑th term of A045544. */
seq_t a(idx_t n)
{
    static seq_t table[31];
    static size_t len = 0;

    /* Initialise the table on the first call. */
    if (len == 0) {
        /* Known Fermat primes. */
        const seq_t primes[5] = { (seq_t)3, (seq_t)5, (seq_t)17,
                                  (seq_t)257, (seq_t)65537 };
        size_t pos = 0;

        /* Enumerate all non‑empty subsets of the 5 primes. */
        for (idx_t mask = 1; mask < ((idx_t)1 << 5); ++mask) {
            seq_t prod = (seq_t)1;
            for (int bit = 0; bit < 5; ++bit) {
                if (mask & ((idx_t)1 << bit))
                    prod = prod * primes[bit];
            }
            table[pos++] = prod;
        }
        len = pos;
        qsort(table, len, sizeof(seq_t), cmp_seq);
    }

    if (n == 0 || (idx_t)n > (idx_t)len)
        return (seq_t)0;
    return table[(size_t)(n - 1)];
}
