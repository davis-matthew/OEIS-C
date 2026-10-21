/*
 * A075017  Smallest k such that the concatenation k, k+1,k+2,k+3 is divisible
 *          by n; or 0 if no such number exists.
 * https://oeis.org/A075017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that concat(k,k+1,k+2,k+3) ≡ 0 (mod
 *            n); Computed by brute‑force search up to 200·n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   ndigits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ndigits_uint64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety, not used by OEIS offset 1 */
    unsigned long long mod = (unsigned long long)n;

    /* pre‑compute 10^d mod n for d = 0 … 19 */
    unsigned long long pow10[20];
    pow10[0] = 1 % mod;
    for (int i = 1; i < 20; ++i) pow10[i] = (pow10[i-1] * 10ULL) % mod;

    /* search bound – 200·n is sufficient for n ≤ 10 000 */
    unsigned long long limit = mod * 200ULL;
    if (limit <= 1) limit = 1;      /* handle tiny n safely */

    for (unsigned long long k = 1; k <= limit; ++k) {
        unsigned long long rem = 0;
        unsigned long long cur = k;
        for (int step = 0; step < 4; ++step) {
            int len = ndigits_uint64(cur);
            rem = (rem * pow10[len] + (cur % mod)) % mod;
            ++cur;
        }
        if (rem == 0) return (seq_t)k;
    }
    return 0;  /* no solution found within the bound */
}
