/*
 * A078401  Triangle read by rows: T(n,k) = number of numbers <= k that are
 *          coprime to n, 1 <= k <= n.
 * https://oeis.org/A078401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r, c) * where r is the row such that (r-1)*r/2 < n ≤
 *            r*(r+1)/2, * c = n - (r-1)*r/2, * and T(r, c) is the number of
 *            integers ≤ c that are coprime to r. * T(r, c) is computed by
 *            inclusion–exclusion over the prime factors of r: * T(r, c) =
 *            Σ_{d|r} μ(d)·⌊c/d⌋, * where μ(d) = 0 if d is divis...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t, if needed */

/* Count integers ≤k that are coprime to n   (n,k ≥ 1) */
static seq_t
coprime_count(idx_t n, idx_t k)
{
    if (k == 0) return 0;
    if (n == 1) return (seq_t)k;   /* all numbers ≤k are coprime to 1 */

    /* gather distinct prime factors of n */
    idx_t pf[64];
    int pcnt = 0;
    idx_t tmp = n;

    if (tmp % 2 == 0) {
        pf[pcnt++] = 2;
        while (tmp % 2 == 0) tmp /= 2;
    }
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            pf[pcnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) pf[pcnt++] = tmp;   /* remaining prime factor */

    /* inclusion–exclusion over square‑free divisors */
    seq_t result = 0;
    int subsets = 1 << pcnt;       /* number of subsets of prime factors */
    for (int mask = 0; mask < subsets; ++mask) {
        idx_t d = 1;
        int bits = 0;
        for (int i = 0; i < pcnt; ++i) {
            if (mask & (1 << i)) {
                d *= pf[i];
                bits++;
            }
        }
        seq_t val = (seq_t)(k / d);
        if (bits & 1) result -= val;
        else          result += val;
    }
    return result;
}

/* Triangle read by rows: a(n) = T(r, c) as defined above */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n=0 is undefined but return 0 */

/* Find row r: smallest r with r*(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t sum = 1;
    while (sum < n) {
        ++r;
        sum += r;
    }

/* Column index c in that row */
    idx_t col = n - (r - 1) * r / 2;

/* Return the coprime count for (r, col) */
    return coprime_count(r, col);
}
