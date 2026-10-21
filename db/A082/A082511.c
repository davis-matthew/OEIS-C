/*
 * A082511  a(n) = 3^n mod 2n.
 * https://oeis.org/A082511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n mod 2n
 * Range:     u8 n <= 141, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the behavior is irrelevant; return 0 */
    if (n == 0) return (seq_t)0;
    
    /* modulus = 2 * n, computed in seq_t to avoid overflow */
    seq_t mod = (seq_t)n;
    mod = mod + mod;           /* mod = 2*n */
    
    /* fast modular exponentiation: compute 3^n mod mod */
    seq_t base  = (seq_t)3;
    seq_t res   = (seq_t)1;
    idx_t exp   = n;           /* exp fits in the index type */
    
    while (exp > 0) {
        if (exp & 1)    res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    
    /* The result is already < mod */
    return res;
}
