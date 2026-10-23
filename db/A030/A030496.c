/*
 * A030496  Permutations of 1,2,...,n for n=1,2,3,..., arranged
 *          antilexicographically.
 * https://oeis.org/A030496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9739, u16 n <= 9739, u32 n <= 9739, u64 n <= 9739,
 *            u128 n <= 9739, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030496: concatenate all permutations of 1..k in antilexicographic order
   (reverse lexicographic) for k = 1, 2, 3, …  and return the n‑th element of
   that infinite sequence.  a(1) = 1, a(2) = 2, a(3) = 1, a(4) = 1, a(5) = 2, …  */

#include <stdint.h>   /* for UINT64_MAX */

seq_t a(idx_t n)
{
    /* Find the block size m such that the n-th element lies in
       the block of permutations of 1..m.  sum stores the total number
       of elements contributed by blocks for sizes < m. */
    idx_t m     = 0;
    idx_t sum   = 0;          /* elements before block m */
    idx_t fact  = 1;          /* fact = (m-1)! before multiplication */

    for (m = 1; ; ++m) {
        fact *= m;            /* fact = m! now */

        /* Check whether the block size block = fact * m would overflow.
           If it would, the block is larger than any remaining number and
           n necessarily lies in this block. */
        if (fact > (UINT64_MAX - sum) / m) {
            /* block would overflow 64‑bit, hence it covers all remaining n */
            break;
        }

        idx_t block = fact * m;    /* safe multiplication */
        if (sum + block >= n) {
            /* n lies in this block */
            break;
        }
        sum += block;
    }

    /* Off‑by‑one: offset0 is the zero–based index within the current block */
    idx_t offset0  = n - sum - 1;      /* 0 ≤ offset0 < block */
    idx_t perm_id  = offset0 / m;      /* which permutation (in reverse order) */
    idx_t pos_in_p = offset0 % m;      /* which position inside the permutation */

    /* Convert the reverse rank to lexicographic rank. */
    idx_t rank_std = fact - 1 - perm_id;  /* fact = m! */

    /* Pre‑compute factorials up to m to speed up unranking. */
    idx_t fact_arr[64];
    fact_arr[0] = 1;
    for (idx_t i = 1; i <= m; ++i) fact_arr[i] = fact_arr[i - 1] * i;

    /* Prepare the list of available digits 1..m. */
    idx_t digits[64];
    for (idx_t i = 0; i < m; ++i) digits[i] = i + 1;

    /* Unrank the permutation in lexicographic order and return the
       element at position pos_in_p. */
    idx_t r = rank_std;
    for (idx_t i = 0; i < m; ++i) {
        idx_t blocksize = fact_arr[m - 1 - i];
        idx_t index = r / blocksize;
        r %= blocksize;

        idx_t val = digits[index];
        if (i == pos_in_p) {
            return (seq_t)val;
        }

        /* Remove the used digit from the list. */
        for (idx_t j = index; j + 1 < m - i; ++j) {
            digits[j] = digits[j + 1];
        }
    }

    /* The function must always return inside the loop. */
    return (seq_t)0;   /* fallback, should not be reached */
}
