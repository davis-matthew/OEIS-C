/*
 * A117304  Numbers with an even number of digits such that the second half is
 *          twice the first half.
 * https://oeis.org/A117304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^k + 2) * (10^(k-1) + (n – cum_{k-1}) – 1), where k is
 *            the smallest integer such that cum_{k-1} < n ≤ cum_k, and cum_k
 *            = Σ_{i=1..k} (2 * 10^i / 5) is the total count of numbers with
 *            up to 2k digits
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A117304 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* protect against zeroth index */
    seq_t sn = (seq_t)n;             /* use seq_t for arithmetic */

    seq_t pow10k = 10;               /* 10^k, starting with k = 1 */
    seq_t cum = 0;                   /* cumulative count */

    /* find the block (k) containing the index n */
    while (1) {
        seq_t count_k = (seq_t)2 * (pow10k / 5);  /* = 2*10^k/5 */
        if (cum + count_k >= sn) break;
        cum += count_k;
        pow10k *= 10;               /* advance to next power of 10 */
    }

    seq_t idx_in_block = sn - cum - 1;          /* 0‑based offset */
    seq_t minX = pow10k / 10;                   /* 10^(k-1) */
    seq_t X = minX + idx_in_block;             /* first half of the number */
    seq_t result = X * (pow10k + 2);            /* concatenate 2X to X */
    return result;
}
