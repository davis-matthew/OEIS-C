/*
 * A070681  Smallest m in range 1..phi(2n+1) such that 6^m == 1 mod 2n+1, or 0
 *          if no such number exists.
 * https://oeis.org/A070681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m (1 ≤ m ≤ φ(2n+1)) such that 6^m ≡ 1 (mod
 *            2n+1), or 0 if no such m exists
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute φ(m) for all odd 1 ≤ m ≤ 20001 (the largest 2n+1 for n≤10000) */
    static bool init = false;
    static uint32_t phi[20002];

    if (!init) {
        const uint32_t MAX = 20001;
        for (uint32_t i = 0; i <= MAX; ++i) phi[i] = i;
        for (uint32_t i = 2; i <= MAX; ++i) {
            if (phi[i] == i) {                 /* i is prime */
                for (uint32_t j = i; j <= MAX; j += i)
                    phi[j] -= phi[j] / i;
            }
        }
        init = true;
    }

    uint64_t p = 2ULL * (uint64_t)n + 1ULL;          /* modulus 2n+1 */

    /* If p contains a factor 3, 6^m ≡ 0 (mod 3) ≠ 1, hence impossible. */
    if (p % 3ULL == 0ULL) return 0;

    /* Retrieve φ(p) – safe because p ≤ 20001 for the required range */
    uint32_t phi_p = phi[(uint32_t)p];

    /* Search for the smallest exponent m.  All intermediate values fit in 64 bit. */
    uint64_t cur = 1ULL % p;          /* 6^0 ≡ 1 (mod p) */
    for (uint32_t m = 1; m <= phi_p; ++m) {
        cur = (cur * 6ULL) % p;       /* compute 6^m mod p */
        if (cur == 1ULL) return (seq_t)m;
    }
    return 0;                         /* no m found within φ(p) */
}
