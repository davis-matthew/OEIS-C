/*
 * A015931  Positive integers n such that 2^n (mod n) == 2^9 (mod n).
 * https://oeis.org/A015931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer k such that 2^k ≡ 512 (mod k)
 * Range:     u8 n <= 25, u16 n <= 1064, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast modular exponentiation: returns 2^exp mod mod_val  (mod_val > 0) */
static inline idx_t pow2_mod(idx_t mod_val, idx_t exp)
{
    idx_t result = 1 % mod_val;
    idx_t base   = 2 % mod_val;
    while (exp > 0) {
        if (exp & 1U)
            result = (idx_t)(((__uint128_t)result * base) % mod_val);
        base = (idx_t)(((__uint128_t)base * base) % mod_val);
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n<1; return 0 to avoid UB */

    idx_t found = 0;    /* number of terms already found */
    idx_t k = 0;        /* current integer being tested */

    while (found < n) {
        k++;
        if (pow2_mod(k, k) == (idx_t)(512 % k)) {
            found++;
            if (found == n)
                return (seq_t)k;   /* cast is OK for both integral and
                                      arbitrary‑precision seq_t types */
        }
    }
    return (seq_t)0;   /* unreachable, but keeps compiler happy */
}
