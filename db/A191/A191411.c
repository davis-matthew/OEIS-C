/*
 * A191411  Class number, k, of n; i.e., imaginary quadratic fields negated
 *          Q(sqrt(-n))=k, or 0 if n is not squarefree (A005117).
 * https://oeis.org/A191411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = class number of the imaginary quadratic field
 *            Q(sqrt(-n)). If n is not squarefree the result is 0. The class
 *            number is computed by enumerating reduced positive‑definite
 *            binary quadratic forms of discriminant D, where D = -n if n ≡ 3
 *            (mod 4) and D = -4n otherwise
 * Range:     u8 n <= 56, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   _floor_isqrt_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"              /* use for gcd if needed */
#include "../utils/is_prime.h"
#include "../utils/_floor_isqrt_u64.h"

/* helper: test whether n is squarefree (trial division, n ≤ 10^6 is fine) */
static bool is_squarefree(idx_t n)
{
    if (n <= 1) return true;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
                if (cnt > 1) return false;
            }
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    /* follow OEIS offset: index starts at 1 */
    if (n < 1) return 0;

    /* zero if n is not squarefree */
    if (!is_squarefree(n)) return 0;

    /* discriminant D (absolute value) */
    idx_t absD;
    if (n % 4 == 3)               /* D = -n */
        absD = n;
    else                          /* D = -4n */
        absD = 4 * n;

    seq_t count = 0;               /* class number */
    /* bound for a in reduced form: a^2 ≤ |D|/3  */
    idx_t bound = (idx_t)_floor_isqrt_u64(absD / 3 + 1);

    for (idx_t aVal = 1; aVal <= bound; ++aVal) {
        /* parity condition: b must have same parity as D (≡ absD mod 2) */
        int parity = absD & 1;
        /* range for b: |b| ≤ a */
        for (int b = -(int)aVal; b <= (int)aVal; ++b) {
            if ((b & 1) != parity) continue;

            seq_t bSq = (seq_t)b * (seq_t)b;
            seq_t rem  = bSq + absD;          /* b^2 + |D| */
            seq_t mod  = 4 * (seq_t)aVal;
            if (rem % mod != 0) continue;     /* not integral c */

            seq_t c = rem / mod;
            if (aVal > (idx_t)c) continue;    /* reduced condition a ≤ c */

            /* further reduced condition when on the boundary */
            if ((aVal == (idx_t)c || aVal == (idx_t)abs(b)) && b < 0)
                continue;

            /* form satisfies all conditions -> class number increment */
            ++count;
        }
    }
    return count;
}
