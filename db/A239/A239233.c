/*
 * A239233  Smaller member in a set of two distinct odd numbers the sum of
 *          whose reciprocals is 2/(2*n - 1), or 0 if no such integer exists.
 * https://oeis.org/A239233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal odd a < b such that 1/a + 1/b = 2/(2*n-1).
 *            Returns 0 if no such pair exists. The algorithm enumerates all
 *            odd divisors u of d² (where d = 2*n‑1) with u < d and u ≡ d (mod
 *            4), then produces a = (u + d) / 2. The smallest such a is
 *            returned
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Recursive helper to generate all divisors of d². */
static void gen_divisors(size_t idx, size_t pf_cnt,
                         seq_t cur, seq_t d,
                         const seq_t *pf_primes, const int *pf_exps,
                         seq_t *best_a, bool *found)
{
    if (idx == pf_cnt) {
        if (cur < d && (cur % (seq_t)4 == d % (seq_t)4)) {
            seq_t a = (cur + d) / (seq_t)2;
            if (!*found || a < *best_a) {
                *best_a = a;
                *found   = true;
            }
        }
        return;
    }
    seq_t prod = cur;
    int max_e  = pf_exps[idx] * 2;  /* exponent in d² */
    for (int e = 0; e <= max_e; ++e) {
        gen_divisors(idx + 1, pf_cnt,
                     prod, d, pf_primes, pf_exps,
                     best_a, found);
        if (e == max_e) break;
        prod *= pf_primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n <= 2)          /* n = 1 or 2 always give 0 */
        return 0;

    seq_t d = (seq_t)n * (seq_t)2 - (seq_t)1;   /* d = 2*n-1, odd */

    /* Factor d by trial division (d <= 20000 for n up to 10000). */
    idx_t temp = (idx_t)d;
    seq_t pf_primes[32];
    int    pf_exps[32];
    size_t pf_cnt = 0;

    /* Odd factors only; d is odd. */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pf_primes[pf_cnt] = (seq_t)p;
            pf_exps[pf_cnt]   = e;
            ++pf_cnt;
        }
    }

    if (temp > 1) {   /* remaining prime factor */
        pf_primes[pf_cnt] = (seq_t)temp;
        pf_exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    /* Enumerate odd divisors of d². */
    seq_t best_a = (seq_t)-1;   /* placeholder for minimal value */
    bool  found  = false;
    gen_divisors(0, pf_cnt, (seq_t)1, d, pf_primes, pf_exps, &best_a, &found);

    return found ? best_a : (seq_t)0;
}
