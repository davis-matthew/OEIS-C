/*
 * A108297  Least positive k such that k * n^2 ends with n, or 0 no such k
 *          exists.
 * https://oeis.org/A108297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive k such that k * n^2 ends with n in decimal
 *            (return 0 if no such k exists). For n = 0 the value is defined
 *            to be 1
 * Range:     u8 n <= 15, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   digit_len, gcd_any, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/gcd_any.h"

/* Find the least positive k such that k*n^2 ≡ n (mod 10^d),
   where d is the number of decimal digits of n. */
seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == (idx_t)0U)
        return (seq_t)1;

    /* 1. number of decimal digits of n */
    idx_t d = digit_len(n);

    /* 2. modulus 10^d */
    seq_t mod = pow10(d);

    /* 3. A = n^2, B = n */
    seq_t nn = (seq_t)n;
    seq_t A  = nn * nn;
    seq_t B  = nn;

    /* 4. solve linear congruence A*k ≡ B (mod mod) */
    seq_t g = gcd_any(A, mod);
    if (B % g != (seq_t)0)          /* no solution */
        return (seq_t)0;

    /* reduced variables (coprime) */
    seq_t A_red = A / g;
    seq_t B_red = B / g;
    seq_t mod_red = mod / g;        /* this is > 1 for n>0 */

    /* 5. find modular inverse of A_red modulo mod_red by brute force
       (mod_red ≤ 10^d, at most 10^5 for n up to 10 000, fast enough). */
    seq_t inv = (seq_t)0;
    for (seq_t t = (seq_t)1; t < mod_red; ++t) {
        if ((A_red * t) % mod_red == (seq_t)1) {
            inv = t;
            break;
        }
    }
    if (inv == (seq_t)0)            /* should not happen */
        return (seq_t)0;

    /* 6. minimal positive solution */
    seq_t k0 = (B_red * inv) % mod_red;
    if (k0 == (seq_t)0)  /* k ≡ 0 (mod mod_red), but we need k>0 */
        k0 = mod_red;

    return k0;
}
