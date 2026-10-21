/*
 * A072230  a(n) = n! (mod n^2), that is, n factorial modulo n^2.
 * https://oeis.org/A072230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! (mod n^2)
 * Range:     u8 n <= 16, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* compute n! modulo n^2.  For small seq_t types use 64‑bit
       arithmetic to avoid overflow; for larger seq_t (e.g. 128‑bit
       or arbitrary precision) use seq_t operations directly.  */

    if (n == 0)  /* not used, but harmless */
        return (seq_t)0;

    /* Small seq_t (≤8 bytes) */
    if (sizeof(seq_t) <= 8) {
        uint64_t mod = (uint64_t)n * (uint64_t)n;          /* n^2 fits in 64 bits for n≤10 000 */
        uint64_t prod = 1 % mod;
        for (idx_t i = 2; i <= n; ++i)
            prod = (prod * (uint64_t)i) % mod;
        return (seq_t)prod;
    }

    /* Large seq_t (≥128 bits or big‑integer) */
    seq_t mod_seq = (seq_t)n * (seq_t)n;
    seq_t prod_seq = 1 % mod_seq;
    for (idx_t i = 2; i <= n; ++i)
        prod_seq = (prod_seq * (seq_t)i) % mod_seq;
    return prod_seq;
}
