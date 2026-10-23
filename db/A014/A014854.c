/*
 * A014854  Numbers k that divide s(k), where s(1)=1, s(j)=7*s(j-1)+j.
 * https://oeis.org/A014854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that k divides s(k) where s(1)=1 and
 *            s(j)=7*s(j-1)+j; equivalently 7^(k+1) ≡ 6k+7 (mod 36k)
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n-th element of A014854 */
#include <stdint.h>

/* Fast modular exponentiation for 64‑bit modulus. */
static inline uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (uint64_t)((__uint128_t)result * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, return 0 for safety */

    idx_t k = 1;                   /* current candidate k   */
    idx_t count = 0;               /* number of valid k seen */

    while (1) {
        uint64_t M = 36 * k;        /* modulus 36k fits in 64‑bit for k within limits */
        uint64_t left = mod_pow_u64(7, k + 1, M); /* 7^(k+1) mod 36k */
        uint64_t rhs  = 6 * k + 7;                       /* 6k+7 < 36k for all k≥1 */

        if (left == rhs) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* kth valid number is simply k */
        }
        ++k;
    }
}
