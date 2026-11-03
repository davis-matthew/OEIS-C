/*
 * A102370  "Sloping binary numbers": write numbers in binary under each other
 *          (right-justified), read diagonals in upward direction, convert to
 *          decimal.
 * https://oeis.org/A102370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, base, look
 * Formula:   a(n) = n + Σ_{k≥1, n+k ≡ 0 (mod 2^k)} 2^k
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9587 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;          /* start with n itself */
    for (uint64_t k = 1; k < 64; ++k)
    {
        uint64_t pow = 1ULL << k;     /* 2^k */
        unsigned __int128 sum = (unsigned __int128)n + k;
        if ((sum % pow) == 0)         /* n+k ≡ 0 (mod 2^k) ? */
            result += (seq_t)pow;      /* add the corresponding power of 2 */
    }
    return result;
}
