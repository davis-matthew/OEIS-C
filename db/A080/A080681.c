/*
 * A080681  17-smooth numbers: numbers whose prime divisors are all <= 17.
 * https://oeis.org/A080681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth integer whose prime divisors are all ≤ 17 (the
 *            17‑smooth numbers). The code generates the sequence with a
 *            classic k‑pointer algorithm
 * Range:     u8 n <= 126, u16 n <= 2166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but return 0 */

    static seq_t *seq = NULL;             /* generated sequence         */
    static size_t seq_len = 0;            /* number of elements stored */
    static size_t seq_cap = 0;            /* allocated capacity        */
    static size_t idxs[7] = {0};          /* indices for each prime    */
    static const idx_t primes[7] = {2,3,5,7,11,13,17};

    /* initial allocation */
    if (seq == NULL) {
        seq_cap = 8;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;                /* first 17‑smooth number   */
        seq_len = 1;
    }

    /* grow capacity if needed */
    if (seq_len < (size_t)n) {
        size_t needed = (size_t)n;
        if (seq_cap < needed) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 8;
            while (new_cap < needed) new_cap <<= 1;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        /* generate missing terms */
        for (; seq_len < (size_t)n; ++seq_len) {
            /* find next candidate by scanning all primes */
            seq_t next = (seq_t)primes[0] * seq[idxs[0]];
            for (int i = 1; i < 7; ++i) {
                seq_t cand = (seq_t)primes[i] * seq[idxs[i]];
                if (cand < next) next = cand;
            }

            seq[seq_len] = next;

            /* increment indices for all primes that matched the minimum */
            for (int i = 0; i < 7; ++i) {
                seq_t cand = (seq_t)primes[i] * seq[idxs[i]];
                if (cand == next) ++idxs[i];
            }
        }
    }

    return seq[n - 1];
}
