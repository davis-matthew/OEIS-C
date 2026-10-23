/*
 * A176562  a(n) = A175341(n)/4.
 * https://oeis.org/A176562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i,j) with 1 ≤ i ≤ n, 0 ≤ j ≤ n, * i² +
 *            j² ≤ n² and gcd(i,j) = 1. This equals * A305117(n²) as defined
 *            in the OEIS, i.e. * a(n) = A305117(n²) = A175341(n)/4
 * Range:     u8 n <= 15, u16 n <= 369, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(x)) by binary search, using the fact that
 * sqrt(x) ≤ n when x ≤ n².  n is the argument of the main function
 * and serves as an upper bound for the square root. */
static inline idx_t floor_sqrt_seq(seq_t x, idx_t n)
{
    if (x == 0 || x == 1)
        return (idx_t)x;
    idx_t low = 0, high = n;                    /* high = n ≥ sqrt(x) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t prod = (seq_t)mid * (seq_t)mid;
        if (prod == x)
            return mid;
        else if (prod < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                                 /* floor of sqrt */
}

/* Count integers ≤ m that are coprime to i using inclusion–exclusion.
 * Distinct prime factors of i are supplied in the array `pr`.  */
static inline seq_t coprime_count(idx_t m, const uint32_t *pr, int pr_cnt)
{
    if (m == 0)
        return 0;
    int64_t not_coprime = 0;
    int subsets = 1 << pr_cnt;
    for (int mask = 1; mask < subsets; ++mask) {
        uint64_t prod = 1;
        int bits = 0;
        for (int b = 0; b < pr_cnt; ++b) {
            if (mask & (1 << b)) {
                prod *= pr[b];
                ++bits;
                if (prod > (uint64_t)m) /* product exceeds m early */
                    break;
            }
        }
        if (prod > (uint64_t)m)
            continue;
        int64_t term = m / prod;
        if (bits & 1)
            not_coprime += term;
        else
            not_coprime -= term;
    }
    return (seq_t)(m - not_coprime);
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Temporary array to hold distinct prime factors of i. */
    uint32_t primes[16];          /* i ≤ 10000, so at most 6 distinct primes */
    int prime_cnt;

    for (idx_t i = 1; i <= n; ++i) {
        /* Maximum j allowed for this i: floor(sqrt(n² - i²)) */
        seq_t R = (seq_t)n * (seq_t)n - (seq_t)i * (seq_t)i;
        idx_t m = floor_sqrt_seq(R, n);

        /* Factor i to obtain its distinct prime divisors. */
        uint32_t tmp = (uint32_t)i;
        prime_cnt = 0;
        if (tmp > 1) {
            if (tmp % 2 == 0) {
                primes[prime_cnt++] = 2;
                while (tmp % 2 == 0) tmp /= 2;
            }
            for (uint32_t p = 3; (uint32_t)p * p <= tmp; p += 2) {
                if (tmp % p == 0) {
                    primes[prime_cnt++] = p;
                    while (tmp % p == 0) tmp /= p;
                }
            }
            if (tmp > 1)
                primes[prime_cnt++] = tmp;
        }

        /* Count j > 0 that are coprime to i. */
        seq_t coprime_j = coprime_count(m, primes, prime_cnt);

        /* j == 0 is allowed only for i == 1 (gcd(1,0) == 1). */
        if (i == 1)
            coprime_j += 1;

        result += coprime_j;
    }

    return result;
}
