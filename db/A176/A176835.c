/*
 * A176835  Number of positive integers k for which k^2 - n*k is a square.
 * https://oeis.org/A176835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36959, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9928 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A176835
 * a(n) = number of positive integers k for which k^2 – n*k is a square.
 * The implementation counts all divisor pairs (a,b) of n² such that
 *  - a and b have the same parity,
 *  - (a + b + 2*n) is divisible by 4,
 *  - and a ≤ b.  Each such pair corresponds to one valid k.
 * The recursion enumerates the divisors of n² directly from the prime
 * factorisation of n, avoiding any large temporary products.
 *
 * Complexity:  O(number of divisors of n²).
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper that recursively enumerates divisor exponents of n².  */
static void count_divisor_pairs(idx_t pos, size_t num,
                                const idx_t *primes, const idx_t *exp2, idx_t *cur_exp,
                                idx_t n, idx_t e2_n, size_t pos_of_2,
                                seq_t *cnt)
{
    if (pos == num) {
        /* Compute parity of a (exponent of 2 in a).  */
        int e2_a = 0;
        if (pos_of_2 != (size_t)-1)
            e2_a = cur_exp[pos_of_2];

        /* Parity condition: same parity of a and b.  */
        bool parity_ok;
        if (e2_n == 0) {
            parity_ok = (e2_a == 0);          /* both odd */
        } else {
            int e2_b = (int)(2 * e2_n) - e2_a;
            parity_ok = (e2_a >= 1) && (e2_b >= 1); /* both even */
        }
        if (!parity_ok)
            return;

        /* Compute a mod 4.  */
        unsigned a_mod = 1;
        for (size_t i = 0; i < num; ++i) {
            idx_t exp = cur_exp[i];
            if (primes[i] == 2) {
                unsigned factor = (exp == 0) ? 1U : (exp == 1 ? 2U : 0U);
                a_mod = (a_mod * factor) & 3U;
                if (a_mod == 0U)
                    break;
            } else {
                if ((primes[i] & 3) == 3U && (exp & 1)) {
                    a_mod = (a_mod * 3U) & 3U;
                }
            }
        }

        /* Compute b mod 4.  */
        unsigned b_mod = 1;
        for (size_t i = 0; i < num; ++i) {
            idx_t exp_b = exp2[i] - cur_exp[i];
            if (primes[i] == 2) {
                unsigned factor = (exp_b == 0) ? 1U : (exp_b == 1 ? 2U : 0U);
                b_mod = (b_mod * factor) & 3U;
                if (b_mod == 0U)
                    break;
            } else {
                if ((primes[i] & 3) == 3U && (exp_b & 1)) {
                    b_mod = (b_mod * 3U) & 3U;
                }
            }
        }

        /* Check divisibility by 4: (a + b + 2*n) % 4 == 0.  */
        unsigned two_n_mod4 = (n & 1) ? 2U : 0U;  /* 2*n mod 4 */
        if (( (a_mod + b_mod + two_n_mod4) & 3U ) == 0U)
            (*cnt)++;                         /* a pair satisfies all conditions */
        return;
    }

    /* Explore exponents for primes[pos] from 0 to exp2[pos].  */
    for (idx_t e = 0; e <= exp2[pos]; ++e) {
        cur_exp[pos] = e;
        count_divisor_pairs(pos + 1, num, primes, exp2, cur_exp,
                            n, e2_n, pos_of_2, cnt);
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, but return 0 */

    /* Factorise n: store primes and exponents.  */
    idx_t primes[64];
    idx_t exp_n[64];
    size_t uniq = 0;
    idx_t m = n;
    size_t pos_of_2 = (size_t)-1;
    idx_t e2_n = 0;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[uniq] = p;
            exp_n[uniq] = e;
            if (p == 2) {
                pos_of_2 = uniq;
                e2_n = e;
            }
            ++uniq;
        }
    }
    if (m > 1) {
        primes[uniq] = m;
        exp_n[uniq] = 1;
        if (m == 2) {
            pos_of_2 = uniq;
            e2_n = 1;
        }
        ++uniq;
    }

    /* exp2[i] = 2 * exp_n[i] for n² */
    idx_t exp2[64];
    for (size_t i = 0; i < uniq; ++i)
        exp2[i] = exp_n[i] * 2;

    idx_t cur_exp[64] = {0};

    seq_t count = 0;
    count_divisor_pairs(0, uniq, primes, exp2, cur_exp,
                        n, e2_n, pos_of_2, &count);

    /* Each valid pair (a,b) with a < b is counted twice; a=b (i.e. a=n)
     * is counted once.  Thus correct result is (count + 1) / 2.  */
    return (count + 1) / 2;
}
