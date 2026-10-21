/*
 * A079115  Least k such that n+k is squarefree and coprime to n.
 * https://oeis.org/A079115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that n+k is squarefree and gcd(n,n+k)=1
 * Range:     u8 n <= 753, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Euclidean algorithm for seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Test if a 64‑bit integer is squarefree */
static bool is_squarefree_uint64(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        uint64_t pp = p * p;
        if (x % pp == (uint64_t)0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t N = n;
    for (idx_t k = 1; ; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t M = N + kseq;

        /* Check squarefree using 64‑bit conversion (safe for small n) */
        uint64_t m64 = (uint64_t)M;
        if (is_squarefree_uint64(m64) && gcd_seq(N, M) == (seq_t)1)
            return kseq;   /* k fits in seq_t for the tested ranges */
    }
}
