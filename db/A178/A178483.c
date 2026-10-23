/*
 * A178483  For n=1,2,... list all products of the first n primes raised to
 *          some nonnegative power less than n.
 * https://oeis.org/A178483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the concatenated sequence of all
 *            products Π_{i=1}^{k} p_i^{e_i} with 0 ≤ e_i < k, where p_i is
 *            the ith prime. The sequence is enumerated by increasing k, and
 *            for each k the exponent tuples are given in base‑k
 *            representation with the least significant digit (exponent of...
 * Range:     u8 n <= 28, u16 n <= 158, u32 n <= 1532, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

static const uint64_t small_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
    29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL,
    61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL,
    101ULL, 103ULL, 107ULL, 109ULL, 113ULL, 127ULL, 131ULL,
    137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL,
    173ULL, 179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL
};

/* Compute i^i using seq_t arithmetic.  i is small enough that no
   overflow occurs for the ranges we use. */
static seq_t pow_self(idx_t i)
{
    seq_t r = 1;
    for (idx_t k = 0; k < i; ++k)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    /* Locate the block k such that the cumulative sum of block sizes
       up to k-1 < n ≤ cumulative sum up to k.  We keep n as a zero‑
       based offset within the chosen block. */
    idx_t  block = 1;          /* current block size index k */
    idx_t  m = n;              /* remaining offset  (1‑based)   */

    while (1) {
        seq_t blockSize = pow_self(block);
        if ((seq_t)m <= blockSize)
            break;
        m -= (idx_t)blockSize;
        ++block;
    }

    /* Offset within the block (0‑based). */
    seq_t offset = (seq_t)m - 1;

    /* Decode offset into base‑block digits: exponents for primes.
       The least significant digit corresponds to the first prime (2). */
    seq_t product = 1;
    idx_t  exp;
    for (idx_t i = 0; i < block; ++i) {
        exp = (idx_t)(offset % block);
        offset /= block;
        product *= pow_uint(small_primes[i], exp);
    }
    return product;
}
