/*
 * A059379  Array of values of Jordan function J_k(n) read by antidiagonals
 *          (version 1).
 * https://oeis.org/A059379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = J_k(m) read along anti‑diagonals of the table J_k(n)
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 163, u64 n <= 423,
 *            u128 n <= 1113, big any n
 * Validated: standard, pass, 75 terms through n = 9937 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Factorise a small positive integer m (m ≤ few hundred).
   Stores distinct prime factors in pr[], corresponding exponents in ex[],
   and returns the count in *cnt. */
static void factor_small(uint64_t m, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t n = m;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Small integer power for seq_t.  Works for built‑in integer types
   and for C++ arbitrary‑precision classes with overloaded *. */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Safeguard for the undefined offset 0 */
    if (n == 0) return 0;

    /* Find diagonal s with cumulative(s-1) < n ≤ cumulative(s) */
    idx_t s = 2;
    idx_t cumulative = 1;          /* cumulative up to diagonal s */
    while (cumulative < n) {
        ++s;
        cumulative += s - 1;       /* add number of elements on new diagonal */
    }
    idx_t prev_cumulative = cumulative - (s - 1);
    idx_t offset = n - prev_cumulative;   /* 1 … s-1 */
    idx_t k = offset;           /* row index (k)  */
    idx_t m = s - k;            /* column index (m) */

    if (m == 1) return 1;       /* J_k(1) = 1 */

    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt;

    factor_small(m, primes, exponents, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = (seq_t)primes[i];
        idx_t e = (idx_t)exponents[i];
        idx_t exp1 = k * e;
        idx_t exp2 = k * (e - 1);
        seq_t term = power_seq(base, exp1) - power_seq(base, exp2);
        result *= term;
    }
    return result;
}
