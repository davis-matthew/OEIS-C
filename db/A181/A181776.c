/*
 * A181776  a(n) = lambda(lambda(n)), where lambda(n) is the Carmichael lambda
 *          function (A002322).
 * https://oeis.org/A181776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 561, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A181776 – a(n) = lambda(lambda(n))
 *  (lambda = Carmichael function, A002322)
 *
 * The routine computes the Carmichael lambda of an integer m (m >= 1)
 * using prime factorisation by trial division.  All arithmetic is performed
 * in the type seq_t; the function does not rely on any external gcd routine,
 * instead providing a minimal implementation valid for all supported seq_t
 * types (uint8_t …, arbitrary‑precision integer).
 * A ten‑thousand call effort is well below 2 s as the factorisation limit
 * is sqrt(m) ≤ 100 for all demanded n.
 */

#include <stdint.h>
#include <stddef.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Helper: Euclidean algorithm on seq_t                                 */
static seq_t
seq_gcd_local(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Helper: lcm(a,b) using gcd                                         */
static seq_t
seq_lcm_local(seq_t a, seq_t b)
{
    if (!a || !b) return 0;
    seq_t g = seq_gcd_local(a, b);
    return (a / g) * b;
}

/* Helper: integer exponentiation of seq_t (base^exp)                  */
static seq_t
seq_pow_local(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Compute Carmichael λ(m) for m >= 1                                  */
static seq_t
lambda_calc(idx_t m)
{
    if (m == 1) return 1;

    uint64_t n = (uint64_t)m;
    seq_t lcm = 1;
    uint64_t e;

    /* Handle factor 2 */
    if ((n & 1ULL) == 0) {
        e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        if (e) {
            seq_t term;
            if (e == 1)          term = 1;          /* λ(2)   = 1 */
            else if (e == 2)     term = 2;          /* λ(4)   = 2 */
            else                term = seq_pow_local(2, e - 2); /* λ(2^e) = 2^{e-2} */
            lcm = seq_lcm_local(lcm, term);
        }
    }

    /* Odd prime factors */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            seq_t term;
            if (e == 1) {
                term = (seq_t)(p - 1);                    /* λ(p)   = p-1      */
            } else {
                seq_t pow = seq_pow_local((seq_t)p, e - 1);
                term = pow * (seq_t)(p - 1);              /* λ(p^e) = p^{e-1}(p-1) */
            }
            lcm = seq_lcm_local(lcm, term);
        }
    }

    /* Remaining prime factor >1 */
    if (n > 1) {
        seq_t term = (seq_t)(n - 1);          /* λ(p) = p-1          */
        lcm = seq_lcm_local(lcm, term);
    }

    return lcm;
}

/* --------------------------------------------------------------------- */
/* Public entry point expected by the build system                       */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* not expected for legal n */
    seq_t first  = lambda_calc(n);
    seq_t second = lambda_calc((idx_t)first); /* λ(n) ≤ n, safe cast   */
    return second;
}
