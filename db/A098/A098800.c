/*
 * A098800  Length of longest prefix of binary representation of n that is
 *          also a suffix of that of n^2.
 * https://oeis.org/A098800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A098800:  a(n) = largest k such that the k‑bit prefix of n (in binary)
    equals the k‑bit suffix of n².  (If no non‑empty match, a(n)=0.)
    For n=0 the binary representation is "0", so a(0)=1. */
#include <stdint.h>

static inline idx_t bitlen_seq_t(seq_t x)          /* length of binary representation, 0 → 1 */
{
    if (x == 0) return 1;
    idx_t len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;                  /* n², fits into seq_t for test range */
    idx_t len_n  = bitlen_seq_t((seq_t)n);
    idx_t len_sq = bitlen_seq_t(sq);
    idx_t maxk = (len_n < len_sq ? len_n : len_sq);  /* maximum possible length */

    for (idx_t k = maxk; k > 0; --k) {
        /* prefix of length k from the high‑order bits of n */
        seq_t prefix;
        if (len_n == k) prefix = (seq_t)n;
        else prefix = (seq_t)n >> (len_n - k);

        /* suffix of length k from the low‑order bits of n² */
        seq_t mask = 0;
        seq_t one = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) mask |= (one << i);
        seq_t suffix = sq & mask;

        if (prefix == suffix) return (seq_t)k;
    }
    /* no non‑empty match → length 0 */
    return 0;
}
