/*
 * A085629  Let b(n) equal the product of the exponents in the prime
 *          factorization of n. Then a(n) gives the least k such that b(k) =
 *          n.
 * https://oeis.org/A085629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   cmp_seq_t, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1.
   For n ≥ 2, a(n) is the least integer k such that the product of
   exponents in the prime factorization of k equals n.
   The implementation recursively explores all exponent partitions of n
   and keeps the minimal resulting k. */
#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"
#include "../utils/cmp_seq_t.h"

static const uint64_t primes[64] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
    59ULL, 61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL,
    97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL, 127ULL, 131ULL,
    137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
    179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL,
    227ULL, 229ULL, 233ULL, 239ULL, 241ULL, 251ULL, 257ULL, 263ULL,
    269ULL, 271ULL, 277ULL, 281ULL, 283ULL, 293ULL, 307ULL, 311ULL
};

/* Recursive search of exponent partitions of 'rem'.
   'max_factor' limits the largest exponent allowed to keep non‑increasing order.
   'depth' chooses the prime (primes[depth]) to apply the exponent to.
   'cur' is the product built so far.
   '*best' holds the minimal candidate found; 0 means none found yet. */
static void rec(idx_t rem, idx_t max_factor, unsigned int depth,
                seq_t cur, seq_t *best)
{
    if (rem == 1) {
        if (*best == 0 || cmp_seq_t(&cur, best) < 0) {
            *best = cur;
        }
        return;
    }

    idx_t limit = rem < max_factor ? rem : max_factor;
    for (idx_t f = limit; f >= 2; --f) {
        if (rem % f == 0) {
            seq_t pw  = pow_seq((seq_t)primes[depth], (int)f);
            seq_t prod = seq_mul(cur, pw);
            if (*best != 0 && cmp_seq_t(&prod, best) >= 0) {
                continue; /* prune */
            }
            rec(rem / f, f, depth + 1, prod, best);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* b(1)=1 → a(1)=1 */
    seq_t best = 0;
    rec(n, n, 0, 1, &best);
    return best;
}
