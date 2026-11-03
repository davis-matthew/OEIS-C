/*
 * A174699  Triangle read by rows: T(n,k) = 2^(2n) mod (2k+1).
 * https://oeis.org/A174699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   let r = floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2; a(n) = 4^r
 *            mod (2*k+1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_mod_4(idx_t exp, uint64_t mod)
{
    if (mod == 1) return (seq_t)0;
    uint64_t base = 4 % mod;
    uint64_t res  = 1 % mod;
    while (exp) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (seq_t)res;
}

static inline idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t lo = 0, hi = (idx_t)1 << 32, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid underflow */
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* find row r */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t sqrtv = isqrt_u128(val);
    idx_t r = (sqrtv - 1) >> 1;        /* floor((sqrt(8*n+1)-1)/2) */

    /* find column k */
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - tri;

    /* modulus */
    uint64_t mod = 2 * k + 1;

    /* result = 4^r mod mod */
    return pow_mod_4(r, mod);
}
