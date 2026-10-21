/*
 * A093819  Algebraic degree of sin(2*Pi/n).
 * https://oeis.org/A093819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) if gcd(n,8)<4 * a(n) = φ(n)/4 if gcd(n,8)=4 and
 *            φ(n)≡0 (mod 4) * a(n) = φ(n)/2 if gcd(n,8)=4 and φ(n)≡2 (mod 4)
 *            * a(n) = φ(n)/2 if gcd(n,8)>4 * * This definition reproduces the
 *            OEIS sequence A093819 (Algebraic degree of * sin(2*π/n)) for all
 *            n ≥ 1, including the special case n = 4 whe...
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1; for safety return 0 for n = 0. */
    if (n == 0) return 0;

    /* Compute Euler's totient of n. */
    seq_t phi_n = phi(n);

    /* Determine gcd(n,8) by examining n mod 8. */
    idx_t r = n & 7;          /* n % 8, since 8 is a power of 2 */
    if (r == 0) {             /* gcd(n,8) = 8 */
        return phi_n >> 1;    /* divide by 2 */
    } else if (r == 4) {      /* gcd(n,8) = 4 */
        /* φ(4) = 2 is the only case where φ(n) is not divisible by 4. */
        if ((phi_n & 3) == 0) /* φ(n) ≡ 0 (mod 4) */
            return phi_n >> 2; /* divide by 4 */
        else
            return phi_n >> 1; /* divide by 2 */
    } else if (r == 2 || r == 6) { /* 2 < 4 */
        return phi_n;             /* φ(n) */
    } else { /* r ∈ {1,3,5,7} → gcd = 1 */
        return phi_n;             /* φ(n) */
    }
}
