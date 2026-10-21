/*
 * A218388  Bitwise OR of all divisors of n.
 * https://oeis.org/A218388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = bitwise OR of all divisors of n */
#include <stdint.h>
#include <stdbool.h>

/* Recursive enumeration of all divisors */
static void dfs_divisors(int idx,
                         uint64_t cur,
                         seq_t *res,
                         const uint64_t *pr,
                         const int *exp,
                         int numf)
{
    uint64_t cur_local = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        if (idx == numf - 1) {
            *res |= (seq_t)cur_local;
        } else {
            dfs_divisors(idx + 1, cur_local, res, pr, exp, numf);
        }
        if (e == exp[idx]) break;
        cur_local *= pr[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special case: n == 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Factorize n */
    uint64_t m = (uint64_t)n;
    uint64_t pr[16];          /* prime factors */
    int exp[16];              /* corresponding exponents */
    int numf = 0;             /* number of distinct prime factors */

    /* Handle factor 2 */
    if ((m & 1ULL) == 0) {
        int cnt = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++cnt;
        }
        pr[numf] = 2ULL;
        exp[numf] = cnt;
        ++numf;
    }

    /* Handle odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            pr[numf] = p;
            exp[numf] = cnt;
            ++numf;
            if (numf == 16) break;   /* enough for 64‑bit n */
        }
    }

    if (m > 1) {                /* remaining prime factor */
        pr[numf] = m;
        exp[numf] = 1;
        ++numf;
    }

    /* Enumerate divisors and compute OR */
    seq_t result = 0;
    for (int i = 0; i < numf; ++i) {
        dfs_divisors(i, 1ULL, &result, pr, exp, numf);
    }

    return result;
}
