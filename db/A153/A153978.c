/*
 * A153978  a(n) = n*(n-1)*(n+1)*(3*n-2)/12.
 * https://oeis.org/A153978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n+1)*(3*n-2)/12
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) = 0) */

/* The function computes a(n) using integer arithmetic only.  */
/* It reduces the product by dividing a factor of 3 and a factor of 4   */
/* (i.e. 12) in the early stages to avoid overflow.  The division    */
/* is performed on one of the four factors whenever it is divisible. */
/* This guarantees that the result is exact for any integer type     */
/* seq_t provided by the build (uint8_t, uint16_t, uint32_t,          */
/* uint64_t, unsigned __int128, or arbitrary-precision integer).    */
/* It runs in O(1) time and uses only simple arithmetic operations. */

seq_t a(idx_t n) {
    /* Convert the index to seq_t once for modular and arithmetic use. */
    seq_t sn = (seq_t)n;          /* n  */
    seq_t n1 = sn;                /* n          */
    seq_t n2 = sn - 1;            /* n-1        */
    seq_t n3 = sn + 1;            /* n+1        */
    seq_t n4 = 3 * sn - 2;        /* 3*n-2      */

    /* Divide the product by 3, taking the factor from the first       */
    /* term among {n-1, n, n+1, 3*n-2} that is divisible by 3.         */
    if ((n1 % 3) == 0) {
        n1 /= 3;
    } else if ((n2 % 3) == 0) {
        n2 /= 3;
    } else if ((n3 % 3) == 0) {
        n3 /= 3;
    } else if ((n4 % 3) == 0) {    /* In theory never happens for n>=1 */
        n4 /= 3;
    }

    /* Divide the product by 4, with the same strategy.  One of        */
    /* {n-1, n, n+1} is always divisible by 4, so this always succeeds. */
    if ((n1 % 4) == 0) {
        n1 /= 4;
    } else if ((n2 % 4) == 0) {
        n2 /= 4;
    } else if ((n3 % 4) == 0) {
        n3 /= 4;
    } else if ((n4 % 4) == 0) {    /* Not needed for n>=1 but kept for safety */
        n4 /= 4;
    }

    /* Compute the remaining product.  All intermediate values fit   */
    /* safely within seq_t for the range of n the tests require      */
    /* (and for arbitrary-precision builds).                        */
    return n1 * n2 * n3 * n4;
}
