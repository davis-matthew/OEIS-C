/*
 * A126868  a(n) = (n+1)!! mod n.
 * https://oeis.org/A126868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)!! mod n
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 in the OEIS offset, but guard against 0 */
    if (n == 0)
        return 0;

    /* Compute (n+1)!! modulo n.
     * We use idx_t for all intermediate values so that the modulus
     * can be greater than the final sequence type.  The final result
     * is cast back to seq_t at the end. */
    idx_t mod   = n;
    idx_t prod  = 1;
    idx_t k     = n + 1;

    while (1) {
        prod = (prod * (k % mod)) % mod;
        if (k <= 2)
            break;
        k -= 2;
    }

    return (seq_t)prod;
}
