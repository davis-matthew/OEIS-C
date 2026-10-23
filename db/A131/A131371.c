/*
 * A131371  Number of anagrams of n that are semiprimes.
 * https://oeis.org/A131371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, less, nonn
 * Formula:   a(n) = number of anagrams of n that are semiprimes * * The
 *            computation enumerates all distinct permutations of the decimal
 *            * digits of n that do not begin with 0, builds the corresponding
 *            number, * and counts how many of them are semiprimes (product of
 *            exactly two * primes, counting multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: count prime factors with multiplicity of a 64‑bit integer.
 * The number of prime factors (Ω) must be exactly 2 for a semiprime.
 */
static int count_prime_factors(uint64_t x)
{
    int cnt = 0;
    for (uint64_t p = 2; p * p <= x; ++p) {
        while (x % p == 0) {
            ++cnt;
            x /= p;
        }
    }
    if (x > 1) ++cnt;     /* the remaining prime factor */
    return cnt;
}

/* Recursive generation of distinct permutations of a sorted digit array.
 * `used[i]` indicates whether digit at position i has been used in the
 * current partial permutation.  `perm[pos]` holds the current prefix.
 * `cnt` accumulates the number of semiprime anagrams found.
 */
static void permute_unique(const uint8_t *d, int k, bool *used,
                           uint8_t *perm, int pos, int *cnt)
{
    if (pos == k) {
        if (perm[0] == 0) return;          /* leading zero forbidden */
        /* build the integer value from its digits */
        uint64_t val = 0;
        for (int i = 0; i < k; ++i)
            val = val * 10 + perm[i];
        if (count_prime_factors(val) == 2)
            ++(*cnt);
        return;
    }

    for (int i = 0; i < k; ++i) {
        if (used[i]) continue;
        /* skip duplicates: if the current digit equals the previous
         * digit and the previous one hasn't been used in this position,
         * then this choice would lead to the same permutation as earlier.
         */
        if (i > 0 && d[i] == d[i - 1] && !used[i - 1])
            continue;
        used[i] = true;
        perm[pos] = d[i];
        permute_unique(d, k, used, perm, pos + 1, cnt);
        used[i] = false;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;         /* n is at most 64‑bit */
    /* extract decimal digits into an array (least‑significant first) */
    uint8_t digits[20];
    int k = 0;
    uint64_t tmp = n64;
    if (tmp == 0) return 0;             /* for completeness, though n≥1 */
    while (tmp > 0) {
        digits[k++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* sort digits ascending to enable duplicate avoidance */
    for (int i = 0; i < k - 1; ++i) {
        for (int j = i + 1; j < k; ++j) {
            if (digits[i] > digits[j]) {
                uint8_t t = digits[i];
                digits[i] = digits[j];
                digits[j] = t;
            }
        }
    }

    bool used[20] = {0};
    uint8_t perm[20];
    int cnt = 0;
    permute_unique(digits, k, used, perm, 0, &cnt);

    return (seq_t)cnt;
}
