/*
 * A045087  Numbers whose base-4 representation contains no 1's and exactly
 *          two 2's.
 * https://oeis.org/A045087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-4 representation contains no 1's and exactly two 2's.
   This code computes the nth term of OEIS sequence A045087 by
   combinatorial enumeration without precomputing a lookup table. */
#include <stdint.h>

/* Helper to compute binomial coefficient C(n,k) using 128‑bit arithmetic. */
static unsigned __int128 binom(unsigned int n, unsigned int k) {
    if (k > n) return 0;
    if (k > n-k) k = n-k;
    unsigned __int128 res = 1;
    for (unsigned int i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Count of sequences of length r that contain exactly t digits equal to 2
   and the remaining digits are 0 or 3. */
static unsigned __int128 seq_count(unsigned int r, unsigned int t) {
    if (t > r) return 0;
    unsigned __int128 res = binom(r, t);
    res <<= (r - t);                     /* multiply by 2^(r‑t) */
    return res;
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n) {
    /* Find the length L of the base‑4 representation of the result. */
    unsigned int L = 2;                   /* smallest possible length */
    unsigned __int128 cumul = 0;          /* cumulative count up to L‑1 */
    unsigned __int128 countL;

    /* Count of numbers of length L with exactly two 2’s and no 1’s.
       For L=2 the second subtraction term vanishes because L-1 < 2. */
    countL = binom(L, 2) << (L - 2);
    if (L > 2) countL -= binom(L - 1, 2) << (L - 3);

    while (cumul + countL < n) {          /* move to next length */
        cumul += countL;
        ++L;
        countL = binom(L, 2) << (L - 2);
        if (L > 2) countL -= binom(L - 1, 2) << (L - 3);
    }
    idx_t k = n - (idx_t)cumul;           /* rank within numbers of length L */

    /* Enumerate the digits from most significant to least. */
    int remaining_two = 2;
    seq_t value = 0;

    for (int pos = L - 1; pos >= 0; --pos) {
        int allowed[3];
        int n_allowed = 0;
        if (pos == L - 1) {               /* most significant digit cannot be 0 */
            allowed[n_allowed++] = 2;
            allowed[n_allowed++] = 3;
        } else {
            allowed[n_allowed++] = 0;
            allowed[n_allowed++] = 2;
            allowed[n_allowed++] = 3;
        }

        for (int idx = 0; idx < n_allowed; ++idx) {
            int d = allowed[idx];
            int t_rem = remaining_two - (d == 2 ? 1 : 0);
            unsigned int r_rem = pos;     /* remaining positions after this one */
            unsigned __int128 cnt = seq_count(r_rem, t_rem);

            if ((unsigned __int128)k > cnt) {
                k -= (idx_t)cnt;
            } else {
                value = value * 4 + (seq_t)d;
                remaining_two = t_rem;
                break;
            }
        }
    }
    return value;
}
