/*
 * A166248  a(n) is the absolute value of n minus sum of all the remainders
 *          modulo the numbers below n.
 * https://oeis.org/A166248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166248
   Formula: a(n) = | n - Σ_{k=1..n} (n mod k) |
   Computation uses the identity
        Σ_{k=1..n} (n mod k) = n*n - Σ_{k=1..n} k * ⌊n/k⌋
   The sum k*⌊n/k⌋ is evaluated in O(√n) time by grouping equal
   quotients.  No dynamic memory is allocated, so the function
   works for all supported seq_t types, including arbitrary‑precision
   classes used by the C++ build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, return 0 */

    seq_t sn  = (seq_t)n;          /* cast index to seq_t for calculations */
    seq_t sum = (seq_t)0;          /* Σ (n mod k)  */

    idx_t k = 1;
    while (k <= n) {
        /* quotient q = ⌊n/k⌋ is constant for k in [k, next-1] */
        idx_t q    = n / k;
        idx_t next = n / q + 1;   /* first index where quotient changes */

        idx_t len   = next - k;                           /* block length  */
        seq_t start = (seq_t)k;                           /* first k in block  */
        seq_t last  = (seq_t)(next - 1);                  /* last k in block   */
        seq_t sumk  = (start + last) * (seq_t)len / (seq_t)2; /* Σ k over block */

        /* contribution of the block to Σ (n mod k) */
        sum += (seq_t)len * sn - (seq_t)q * sumk;

        k = next;  /* advance to next block */
    }

    /* a(n) = | n - sum | */
    if (sn >= sum)
        return sn - sum;
    else
        return sum - sn;
}
