/*
 * A160692  Iteration of (k terms=k, followed by 2^k-k terms=0), beginning
 *          with k=0.
 * https://oeis.org/A160692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = f(n,0) * where f(n,k) = if n<2^k then (if n<k then k else
 *            0) else f(n-2^k,k+1) * The implementation iteratively subtracts
 *            powers of two until the * remaining n is less than the next
 *            power, then returns k if the * remainder n is less than k,
 *            otherwise 0
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = 0;
    while (k < 64) {
        idx_t pow = ((idx_t)1ULL << k);
        if (n < pow)
            break;
        n -= pow;
        ++k;
    }
    if (n < k)
        return (seq_t)k;  /* return k if the remaining n is less than k */
    return 0;
}
