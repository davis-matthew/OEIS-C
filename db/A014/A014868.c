/*
 * A014868  Numbers k that divide s(k), where s(1)=1, s(j)=18*s(j-1)+j.
 * https://oeis.org/A014868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute all k such that k divides s(k), where s(1)=1,
 *            s(j)=18*s(j-1)+j. s(k) mod k = 0 iff sum_{i=0}^{k-1} i*18^i ≡ 0
 *            (mod k). Using the closed form T(k) = sum_{i=0}^{k-1} i*18^i =
 *            [(k-1)*18^{k+1} - k*18^k + 18] / 289 , so k | s(k) ⇔ T(k) ≡ 0
 *            (mod k). T(k) is computed modulo k by working modulo M...
 * Range:     u8 n <= 18, u16 n <= 118, u32 n <= 230, u64 n <= 230,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- BEGIN FUNCTION DEFINITION -------------------------------------- */

#include <stdint.h>

static inline uint64_t
powmod_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* M = k * 289;  always fits in uint64_t for the problem size   */
        uint64_t M = (uint64_t)k * 289ULL;

        /* rpow_k  = 18^k   (mod M)   */
        uint64_t rpow_k = powmod_uint64(18ULL, k, M);

        /* rpow_kplus1 = 18^{k+1} (mod M)  */
        uint64_t rpow_kplus1 = (unsigned __int128)rpow_k * 18ULL % M;

        /* term1 = (k-1) * rpow_kplus1  (mod M) */
        uint64_t term1 = ((uint64_t)(k - 1) % M) * rpow_kplus1 % M;

        /* term2 =  k * rpow_k  (mod M) */
        uint64_t term2 = (uint64_t)k % M * rpow_k % M;

        /* N_mod = (term1 - term2 + 18) mod M   */
        uint64_t N_mod = (term1 + M - term2) % M;
        N_mod += 18ULL;
        if (N_mod >= M) N_mod -= M;          /* ensure modulo M */

        /* T = N_mod / 289   (exact division)   */
        uint64_t T = N_mod / 289ULL;

        /* T mod k  must be 0 for k to satisfy the condition   */
        if ((T % k) == 0) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}

/* --- END FUNCTION DEFINITION --------------------------------------- */
