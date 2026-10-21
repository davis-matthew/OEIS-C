/*
 * A226168  Numbers n such that 1/a + 1/b + 1/c + 1/a*b*c = m /(a+b+c) where
 *          a, b and c are the 3 distinct prime divisors of n, and m is a
 *          positive integer such that the equation has infinitely many
 *          solutions.
 * https://oeis.org/A226168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A226168: numbers n such that the set of distinct prime divisors is either
 *     {2,3,7}, {2,5,7} or {3,7,11}.
 *
 * This implementation generates the three Hamming‑type sequences
 * corresponding to the three prime triples, merges them with a
 * simple heap‐free merge, and returns the n‑th value.
 * The algorithm runs in O(n) time and uses O(n) memory for each triple.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0)              /* undefined for n=0, return 0 as sentinel */
        return (seq_t)0;

    /* Number of values that possibly have to be stored for each triple. */
    size_t terms = (size_t)n;

    /* The three prime triples. */
    static const uint64_t primes[3][3] = {
        {2, 3, 7},
        {2, 5, 7},
        {3, 7,11}
    };

    /* Base numbers (product of the three primes). */
    static const seq_t base[3] = {
        (seq_t)42,
        (seq_t)70,
        (seq_t)231
    };

    /* Per‑triple data. */
    seq_t *seq[3];
    size_t cnt[3];       /* number of elements currently stored in seq[t] */
    size_t i0[3], i1[3], i2[3];   /* indices for next multiples in triple t */
    seq_t next_val[3];   /* current smallest available value from each triple */

    /* Allocate storage for each triple. */
    for (int t = 0; t < 3; ++t) {
        seq[t] = (seq_t *)malloc(terms * sizeof(seq_t));
        if (!seq[t]) return (seq_t)0;  /* allocation failure: return 0 */
        seq[t][0] = base[t];
        cnt[t]   = 1;
        i0[t]    = i1[t] = i2[t] = 0;
        next_val[t] = base[t];
    }

    seq_t result = 0;
    size_t produced = 0;

    while (produced < n) {
        /* Find the triple with the smallest next_val. */
        int min_t = 0;
        if (next_val[1] < next_val[min_t]) min_t = 1;
        if (next_val[2] < next_val[min_t]) min_t = 2;

        result = next_val[min_t];
        ++produced;
        if (produced == n)
            break;

        /* Generate the next candidate for the selected triple. */
        seq_t c0 = seq[min_t][i0[min_t]] * (seq_t)primes[min_t][0];
        seq_t c1 = seq[min_t][i1[min_t]] * (seq_t)primes[min_t][1];
        seq_t c2 = seq[min_t][i2[min_t]] * (seq_t)primes[min_t][2];

        /* Select the smallest among them. */
        seq_t nxt = c0;
        if (c1 < nxt) nxt = c1;
        if (c2 < nxt) nxt = c2;

        /* Append to the triple sequence. */
        seq[min_t][cnt[min_t]] = nxt;
        ++cnt[min_t];

        /* Advance indices that contributed to nxt. */
        if (c0 == nxt) ++i0[min_t];
        if (c1 == nxt) ++i1[min_t];
        if (c2 == nxt) ++i2[min_t];

        /* Update the next_val for this triple. */
        next_val[min_t] = nxt;
    }

    /* Free temporary storage. */
    for (int t = 0; t < 3; ++t)
        free(seq[t]);

    return result;
}
