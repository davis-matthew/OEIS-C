/*
 * A015620  Number of triples of different integers from [ 2,n ] with no
 *          common factors between pairs.
 * https://oeis.org/A015620
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples of different integers in [2,n] with
 *            pairwise coprime
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   fast_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/fast_gcd.h"

static inline void set_bit(uint64_t *arr, idx_t idx)
{
    arr[idx >> 6] |= 1ULL << (idx & 63);
}

static inline int test_bit(uint64_t *arr, idx_t idx)
{
    return (arr[idx >> 6] >> (idx & 63)) & 1ULL;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* compute number of 64‑bit words needed for indices 0…n */
    idx_t words = (n >> 6) + 1;          /* (n/64)+1  */
    size_t total_words = (size_t)words * (n + 1);

    /* allocate bitsets for each number */
    uint64_t *fwd = (uint64_t *)calloc(total_words, sizeof(uint64_t)); /* bits > i */
    uint64_t *rev = (uint64_t *)calloc(total_words, sizeof(uint64_t)); /* bits < i */

    if (!fwd || !rev) return 0; /* allocation failure, return 0 */

    /* build tables: for each pair (i,j) with i<j and gcd(i,j)=1 */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t *fwd_i = fwd + (size_t)i * words;
        for (idx_t j = i + 1; j <= n; ++j) {
            if (fast_gcd(i, j) == 1) {
                set_bit(fwd_i, j);          /* j > i  */
                set_bit(rev + (size_t)j * words, i);   /* i < j  */
            }
        }
    }

    seq_t ans = 0;
    for (idx_t c = 3; c <= n; ++c) {
        uint64_t *bc = rev + (size_t)c * words;   /* numbers < c coprime to c */
        for (idx_t a = 2; a < c; ++a) {
            if (test_bit(bc, a)) {                /* a coprime to c */
                uint64_t *fwd_a = fwd + (size_t)a * words;
                /* intersect fwd_a (bits > a) with bc (bits < c) */
                for (idx_t w = 0; w < words; ++w) {
                    ans += (seq_t)__builtin_popcountll(fwd_a[w] & bc[w]);
                }
            }
        }
    }

    free(fwd);
    free(rev);
    return ans;
}
