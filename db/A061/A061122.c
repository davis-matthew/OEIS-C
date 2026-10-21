/*
 * A061122  Number of degree-n permutations of order exactly 8.
 * https://oeis.org/A061122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A(n) – B(n) A(n) = number of permutations of degree n
 *            whose cycle lengths are all in {1,2,4,8} (i.e., order dividing
 *            8) B(n) = number of permutations of degree n whose cycle lengths
 *            are all in {1,2,4} (i.e., order dividing 4) Thus a(n) counts
 *            permutations of degree n whose exact order is 8
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑compute factorials up to n.  */
    seq_t *fact = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i-1] * ((seq_t)i);

    /* dp arrays for permutations with cycle lengths in {1,2,4,8}
       and for those in {1,2,4} respectively.  */
    seq_t *da = (seq_t*)malloc((n + 1) * sizeof(seq_t)); /* A */
    seq_t *db = (seq_t*)malloc((n + 1) * sizeof(seq_t)); /* B */
    da[0] = db[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sumA = (seq_t)0;
        seq_t sumB = (seq_t)0;

        /* k = 1 */
        sumA += da[i-1];
        sumB += db[i-1];

        /* k = 2 */
        if (i >= 2) {
            seq_t mult = fact[i-1] / fact[i-2];          /* = i-1 */
            sumA += mult * da[i-2];
            sumB += mult * db[i-2];
        }

        /* k = 4 */
        if (i >= 4) {
            seq_t mult = fact[i-1] / fact[i-4];
            sumA += mult * da[i-4];
            sumB += mult * db[i-4];
        }

        /* k = 8 (only for A) */
        if (i >= 8) {
            seq_t mult = fact[i-1] / fact[i-8];
            sumA += mult * da[i-8];
        }

        da[i] = sumA;
        db[i] = sumB;
    }

    seq_t result = da[n] - db[n];

    free(fact);
    free(da);
    free(db);
    return result;
}
