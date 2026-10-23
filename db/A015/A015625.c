/*
 * A015625  Quadruples of different integers from [ 1,n ] with no common
 *          factors between triples.
 * https://oeis.org/A015625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of 4‑tuples of distinct integers from [1..n] such that
 *            no triple has a common divisor >1. Computed by
 *            inclusion–exclusion over primes
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   build_primes_upto, dfs_primes, process_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/dfs_primes.h"
#include "../utils/process_set.h"

seq_t a(idx_t n)
{
    /* total count of 4‑tuples with all numbers distinct */
    seq_t good = 0;
    if (n >= 4) {
        good = (seq_t)n * (n - 1) * (n - 2) * (n - 3) / 24;
    }

    uint64_t *prime_list = NULL;
    size_t prime_count = 0;
    build_primes_upto(n, &prime_list, &prime_count);

    /* t is never larger than 5 for n ≤ 10000 – array size 10 is safe */
    idx_t selected[10];

    dfs_primes(n, 0, 1, prime_list, prime_count,
               selected, 0, &good);

    free(prime_list);
    return good;
}
