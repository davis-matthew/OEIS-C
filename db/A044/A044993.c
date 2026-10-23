/*
 * A044993  Numbers whose base-3 representation contains no 0's and exactly
 *          four 2's.
 * https://oeis.org/A044993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers whose base‑3 representation contains no 0's and
 *            exactly four 2's, enumerated in increasing order using
 *            combinatorial unranking of positions of the four 2's
 * Range:     u8 n <= 6, u16 n <= 462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute C(n, k) for small k (k ≤ 4) with 128‑bit arithmetic to avoid overflow. */
static inline idx_t binom_small(idx_t n, int k)
{
    if (k < 0 || (idx_t)k > n)
        return 0;
    if (k > n - k)
        k = (int)(n - k);

    __int128 res = 1;
    for (int i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return (idx_t)res;
}

/* Compute C(n, 4) more concisely. */
static inline idx_t binom4(idx_t n)
{
    if (n < 4)
        return 0;
    __int128 t = (__int128)n * (n - 1) * (n - 2) * (n - 3);
    t /= 24;
    return (idx_t)t;
}

seq_t a(idx_t n)
{
    /* Find the length L of the base‑3 representation that contains the n‑th term. */
    idx_t cumulative = 0;
    size_t L = 4;                  /* length starts at 4 because we need 4 twos */
    while (1) {
        idx_t add = binom4(L);
        if (cumulative + add >= n)
            break;
        cumulative += add;
        ++L;
    }

    /* Rank within blocks of the same length (zero‑based). */
    idx_t rank = n - cumulative - 1;

    /* Unranking: determine which positions hold the digit 2. */
    unsigned long long mask = 0;   /* bit i == 1 means digit 2 at position i (0 = least significant) */
    int twos_left = 4;
    for (long long pos = (long long)L - 1; pos >= 0; --pos) {
        idx_t count_if_one = binom_small((idx_t)pos, twos_left);
        if (rank < count_if_one) {
            /* Digit 1 at this position; keep twos_left unchanged. */
            continue;
        } else {
            /* Digit 2 at this position. */
            rank -= count_if_one;
            mask |= (1ULL << pos);
            --twos_left;
        }
    }

    /* Compute the numeric value: base sum of all 1's plus extra 1 for each 2. */
    seq_t val = 0;
    seq_t pow3 = 1;                 /* 3^0 */
    for (size_t i = 0; i < L; ++i) {
        seq_t contribution = pow3;  /* digit 1 gives 1 * 3^i */
        if (mask & (1ULL << i))
            contribution += pow3;  /* add another 3^i for a digit 2 */
        val += contribution;
        pow3 *= 3;
    }

    return val;
}
