/*
 * A195470  Number of numbers k with 0 <= k < n such that 2^k + 1 is multiple
 *          of n.
 * https://oeis.org/A195470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k : 0 <= k < n, (2^k + 1) mod n = 0 }
 * Range:     u8 n <= 2026, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return 0;          /* undefined, but keep safety */
    if (n == 1) return 1;          /* 2^0 + 1 = 2 ≡ 0 (mod 1) */
    if (n == 2) return 1;          /* 2^0 + 1 = 2 ≡ 0 (mod 2) */
    if ((n & 1) == 0) return 0;    /* even n > 2 cannot satisfy 2^k ≡ -1 (mod n) */

    seq_t count = 0;
    seq_t pow   = 1 % ((seq_t)n);   /* 2^0 mod n */
    seq_t mod_n = (seq_t)n;

    for (idx_t k = 0; k < n; ++k) {
        if (pow == (mod_n - 1)) {  /* check 2^k ≡ -1 (mod n) */
            ++count;
        }
        /* advance: pow = 2^(k+1) mod n */
        pow = (pow * ((seq_t)2)) % mod_n;
    }
    return count;
}
