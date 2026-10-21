/*
 * A237123  Number of ways to write n = i + j + k with 0 < i < j < k such that
 *          phi(i), phi(j) and phi(k) are all cubes, where phi(.) is Euler's
 *          totient function.
 * https://oeis.org/A237123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered triples (i,j,k) with 0<i<j<k, i+j+k =
 *            n and phi(i),phi(j),phi(k) are perfect cubes
 * Range:     u8 n <= 7000, u16 n <= 7000, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Helpers:   is_perfect_cube, totient_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_perfect_cube.h"
#include "../utils/totient_sieve.h"

seq_t a(idx_t n)
{
    /* Allocate memory for totient array and good flag array. */
    uint64_t *phi = (uint64_t*)malloc((n + 1) * sizeof(uint64_t));
    bool    *good = (bool*)malloc((n + 1) * sizeof(bool));
    if (!phi || !good) {
        free(phi); free(good);
        return (seq_t)0;
    }

    /* Compute totients up to n. */
    totient_sieve(n, phi);

    /* Mark numbers with cube totient. */
    size_t good_cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (is_perfect_cube(phi[i])) {
            good[i] = true;
            ++good_cnt;
        } else {
            good[i] = false;
        }
    }

    /* Build a compact list of indices with good[i] == true. */
    if (good_cnt == 0) {
        free(phi); free(good);
        return (seq_t)0;
    }
    idx_t *good_list = (idx_t*)malloc(good_cnt * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i)
        if (good[i]) good_list[pos++] = i;

    /* Count triples (i<j<k) with i+j+k = n. */
    seq_t count = 0;
    for (size_t a = 0; a < good_cnt; ++a) {
        idx_t ia = good_list[a];
        for (size_t b = a + 1; b < good_cnt; ++b) {
            idx_t jb = good_list[b];
            idx_t k   = (idx_t)n - ia - jb;
            if (k > jb && k <= n && k >= 1 && good[k])
                ++count;
        }
    }

    free(phi);
    free(good);
    free(good_list);
    return count;
}
