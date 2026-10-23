/*
 * A198799  Smallest m such that m can be written in exactly n ways as x^2 +
 *          xy + y^2 with 0 <= x <= y.
 * https://oeis.org/A198799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 30, u128 n <= 70,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A198799 – smallest m that has exactly n representations
 *  as  x^2 + xy + y^2  with 0 ≤ x ≤ y.
 *
 *  Based on the factorisation of m in the Eisenstein integers.
 *  If  m = ∏ p_i^{a_i}  with primes  p_i ≡ 1 (mod 3),
 *  the number of unordered representations R(m) is
 *
 *          R(m) = (∏ (a_i + 1) + 1)/2   when  ∏ (a_i + 1) is odd,
 *          R(m) = ∏ (a_i + 1)/2         when  ∏ (a_i + 1) is even.
 *
 *  Hence for a given n we must find the smallest m whose
 *      product (∏ (a_i + 1)) equals 2n−1  (odd choice)  or  2n (even choice).
 *  The routine below searches over non‑increasing exponent vectors
 *  using depth–first search and keeps the minimal product for each target.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp)   */
/* seq_mul is provided by the utilities; it works for both built‑in
 * integer types and the arbitrary‑precision class. */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b)        */

#define PRIMES_1MOD3_COUNT 122
static const uint64_t primes1mod3[PRIMES_1MOD3_COUNT] = {
    7, 13, 19, 31, 37, 43, 61, 67, 73, 79,
    97, 103, 109, 127, 139, 151, 157, 163, 193, 199,
    211, 223, 227, 229, 233, 241, 251, 257, 263, 269,
    271, 277, 283, 307, 313, 331, 337, 349, 367, 373,
    379, 409, 421, 433, 439, 443, 457, 463, 467, 479,
    487, 499, 503, 509, 521, 523, 541, 547, 557, 563,
    569, 571, 577, 587, 593, 599, 607, 613, 617, 619,
    631, 641, 643, 647, 653, 659, 673, 677, 683, 691,
    701, 709, 719, 727, 733, 739, 743, 751, 757, 761,
    769, 773, 787, 797, 809, 811, 823, 827, 829, 839,
    853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
    919, 929, 937, 941, 947, 953, 967, 971, 977, 983,
    991, 997
};

/* depth‑first search over exponents e_i (non‑increasing) that give product rem,
 * cur holds the partially built value of m, best keeps the minimal m found. */
static void dfs_prime(uint64_t idx, uint64_t rem, uint64_t prev,
                      seq_t cur, seq_t *best, bool *found)
{
    if (rem == 1) {
        if (!*found || cur < *best) {
            *best = cur;
            *found = true;
        }
        return;
    }
    if (idx >= PRIMES_1MOD3_COUNT) return;

    uint64_t max_e = (prev < rem - 1) ? prev : rem - 1;
    for (uint64_t e = max_e; e >= 1; --e) {
        if (rem % (e + 1) != 0) continue;

        seq_t p_pow = big_pow((seq_t)primes1mod3[idx], (seq_t)e);
        seq_t next = seq_mul(cur, p_pow);   /* next = cur * primes[idx]^e */

        if (*found && next >= *best) continue;  /* prune larger values */

        dfs_prime(idx + 1, rem / (e + 1), e, next, best, found);
        if (e == 1) break;  /* avoid unsigned underflow in loop */
    }
}

/* returns the smallest m with ∏(a_i+1) == L */
static seq_t minimal_m_for_product(uint64_t L)
{
    if (L == 1) return (seq_t)1;   /* product 1 corresponds to m = 1 */

    seq_t best = 0;
    bool found = false;
    dfs_prime(0, L, L, (seq_t)1, &best, &found);
    return best;
}

seq_t a(idx_t n)
{
    /* special indices coming from the definition of the sequence */
    if (n == 0) return (seq_t)2;   /* smallest m with zero representations */
    if (n == 1) return (seq_t)0;   /* smallest m with one representation */

    uint64_t L1 = 2 * n - 1;   /* odd product case */
    uint64_t L2 = 2 * n;       /* even product case */

    seq_t val1 = minimal_m_for_product(L1);
    seq_t val2 = minimal_m_for_product(L2);

    /* choose the minimal positive m that yields n representations */
    if (val2 != 0 && val2 < val1) return val2;
    return val1;
}
