/*
 * A205686  Prime(A205684(n)), the n-th number s(k) such that 5 divides
 *          s(k)-s(j) for some j<k, where s(j)=prime(j).
 * https://oeis.org/A205686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205686
 * The n-th term is the n-th element of the sequence formed by listing each
 * prime p_k, repeated as many times as there are earlier primes with the
 * same residue modulo 5.  
 * Formally: for each prime p_k let
 *   c_k = |{ j < k : p_j ≡ p_k (mod 5) }|
 * then the sequence consists of p_k repeated c_k times, in order of k.
 * Indexing is 1‑based: a(1)=7, a(2)=13, …
 */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety, n is expected ≥1 */
    idx_t target = n;                      /* 1‑based target position */
    idx_t cumulative = 0;                  /* how many terms seen so far */

    /* For each residue 0…4 keep how many primes seen with that residue */
    int residue_counts[5] = {0};

    /* Search primes in increasing order */
    for (uint64_t num = 2; ; ++num) {
        if (!is_prime_small(num)) continue;   /* skip composites */

        int res = (int)(num % 5);             /* residue modulo 5 */
        idx_t same = (idx_t)residue_counts[res];

        cumulative += same;                   /* add repetitions of this prime */
        if (cumulative >= target)              /* target reached */
            return (seq_t)num;

        /* Now this prime becomes part of the set for future repeats */
        residue_counts[res] = (int)(same + 1);
    }
}
