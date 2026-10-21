/*
 * A086536  Least common multiple of numbers obtained by adding one to the odd
 *          divisors of n and subtracting 1 from the even divisors of n.
 * https://oeis.org/A086536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{ d + 1 : d | n, d odd } ∪ { d − 1 : d | n, d even }
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code below factors n by trial division, enumerates all divisors,
   applies the ±1 transformation, and accumulates the least common
   multiple using the Euclidean algorithm.  It works for any seq_t and
   idx_t defined by the build system. */

#include <stdint.h>
#include <stddef.h>

/* Euclidean algorithm for seq_t values.  Works for any unsigned integer type. */
static inline seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Recursive enumeration of divisors.  cur stores the current divisor. */
static void dfs(int idx, seq_t cur, const idx_t *pr, const int *ex,
                int cnt, seq_t *lcm) {
    if (idx == cnt) {
        seq_t val = (cur & 1U) ? cur + 1 : cur - 1;      /* odd/even adjustment */
        seq_t g   = gcd_seq_t(*lcm, val);
        *lcm      = (*lcm / g) * val;
        return;
    }
    seq_t p = (seq_t)pr[idx];
    seq_t cur_mul = 1;
    for (int k = 0; k <= ex[idx]; ++k) {
        dfs(idx + 1, cur * cur_mul, pr, ex, cnt, lcm);
        cur_mul *= p;
    }
}

seq_t a(idx_t n) {
    /* Factor n */
    idx_t copy = n;
    const int MAX_F = 32;           /* enough for n up to 64‑bit */
    idx_t primes[MAX_F];
    int    exps[MAX_F];
    int    cnt = 0;

    /* factor 2 */
    int e = 0;
    while ((copy & 1ULL) == 0) {
        copy >>= 1;
        ++e;
    }
    if (e) {
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }

    /* factor odd primes by trial division */
    for (idx_t p = 3; p <= copy / p; p += 2) {
        if (copy % p == 0) {
            e = 0;
            while (copy % p == 0) {
                copy /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }

    /* remaining prime > 1 */
    if (copy > 1) {
        primes[cnt] = copy;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Enumerate all divisors and accumulate lcm */
    seq_t lcm = 1;
    dfs(0, (seq_t)1, primes, exps, cnt, &lcm);
    return lcm;
}
