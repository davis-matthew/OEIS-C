/*
 * A067742  Number of middle divisors of n, i.e., divisors in the half-open
 *          interval [sqrt(n/2), sqrt(n*2)).
 * https://oeis.org/A067742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of divisors d of n with sqrt(n/2) <= d < sqrt(2n)
 * Range:     u8 n <= 16, u16 n <= 6893, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8440 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursive helper to count middle divisors */
static void count_middle_divisors(int pos, seq_t curr,
                                  const uint64_t *pr, const uint64_t *ex,
                                  size_t cnt,
                                  seq_t low2, seq_t high2,
                                  seq_t *acc)
{
    if (pos == (int)cnt) {
        seq_t sq = curr * curr;
        if (sq >= low2 && sq < high2) {
            (*acc)++;
        }
        return;
    }
    seq_t val = 1;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        count_middle_divisors(pos + 1, curr * val, pr, ex, cnt,
                              low2, high2, acc);
        val *= pr[pos];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Fast path for n == 1 */
    if (n == 1ULL) return 1;

    /* Factorization of n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            pr[cnt] = p;
            ex[cnt] = e;
            cnt++;
        }
    }
    if (temp > 1) {
        pr[cnt] = temp;
        ex[cnt] = 1;
        cnt++;
    }

    /* Compute bounds */
    seq_t low2  = ((seq_t)n + 1) / 2;          /* ceil(n/2)   */
    seq_t high2 = (seq_t)2 * n;                /* 2*n         */

    seq_t res = 0;
    count_middle_divisors(0, 1, pr, ex, cnt, low2, high2, &res);
    return res;
}
