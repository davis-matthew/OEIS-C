/*
 * A037879  a(n) = (1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*2^i} is the base-2
 *          representation of n and {e(i)} are digits {d(i)} in nondecreasing
 *          order.
 * https://oeis.org/A037879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1‑bits among the first (⌊log2(n)⌋+1 -
 *            popcount(n)) positions of the binary representation of n. For
 *            n≥1 this equals a(n) = popcount( n & ((1ULL << (L -
 *            popcount(n))) - 1) ) where L = floor_log2(n)+1. This implements
 *            the definition a(n) = (1/2) * Σ |d(i)-e(i)| with d(i) the i‑th
 *            b...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute bit length L of n (n>0).  For n==0 we just return 0. */
    if (n == 0) return (seq_t)0;
    idx_t L = 64 - __builtin_clzll(n);        /* number of bits needed to represent n */
    idx_t pop = __builtin_popcountll(n);     /* number of 1‑bits in n */

    idx_t z = L - pop;                       /* number of 0‑bits in the L‑bit string */

    if (z == 0) return (seq_t)0;             /* no 0‑bits to count */

    /* mask to isolate the lowest z bits of n */
    idx_t mask = (z == 64) ? ~((idx_t)0) : ((idx_t)1 << z) - 1;

    idx_t ones_in_low = __builtin_popcountll(n & mask);   /* mis‑placed 1‑bits */

    return (seq_t)ones_in_low;
}
