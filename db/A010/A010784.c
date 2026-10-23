/*
 * A010784  Numbers with distinct decimal digits.
 * https://oeis.org/A010784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = nth integer with all decimal digits distinct (OEIS
 *            A010784)
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Precomputed factorials 0! … 10! */
static const uint64_t fact[11] = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL,
    5040ULL, 40320ULL, 362880ULL, 3628800ULL
};

/* Count of distinct‑digit numbers of each length (1 ≤ L ≤ 10) */
static const uint64_t cnt_len[11] = {
    0ULL,          /* unused */
    10ULL,         /* L = 1  */
    81ULL,         /* L = 2  */
    648ULL,        /* L = 3  */
    4536ULL,       /* L = 4  */
    27216ULL,      /* L = 5  */
    136080ULL,     /* L = 6  */
    544320ULL,     /* L = 7  */
    1632960ULL,    /* L = 8  */
    3265920ULL,    /* L = 9  */
    3265920ULL     /* L =10  */
};

/* Compute nPk = n! / (n-k)!  (n, k ≤ 10) */
static inline uint64_t perm(uint64_t n, uint64_t k)
{
    uint64_t r = 1ULL;
    for (uint64_t i = 0; i < k; ++i)
        r *= (n - i);
    return r;
}

/* ----------  Sequence function  ---------- */
seq_t a(idx_t n)
{
    /* Convert index to native integer */
    uint64_t idx = (uint64_t)n;
    if (idx == 0)
        return (seq_t)0;          /* undefined area – return 0 */

    /* Find the length L of the nth number */
    uint64_t cumulative = 0ULL;
    int L = 0;
    for (int len = 1; len <= 10; ++len) {
        cumulative += cnt_len[len];
        if (idx <= cumulative) {
            L = len;
            break;
        }
    }
    if (L == 0)                 /* index beyond last term */
        return (seq_t)0;

    /* Offset within the group of length L numbers (0‑based) */
    uint64_t offset = idx - (cumulative - cnt_len[L]) - 1ULL;

    /* Build the digits one by one */
    int digits[10];
    int used[10] = {0};
    for (int pos = 0; pos < L; ++pos) {
        int start = (pos == 0 && L > 1) ? 1 : 0;
        for (int d = start; d <= 9; ++d) {
            if (used[d]) continue;
            used[d] = 1;
            uint64_t rem_pos   = (uint64_t)(L - pos - 1);
            uint64_t rem_total = (uint64_t)(10 - (pos + 1));
            uint64_t cnt_missing = perm(rem_total, rem_pos);
            if (offset < cnt_missing) {
                digits[pos] = d;
                break;
            } else {
                offset -= cnt_missing;
                used[d] = 0;
            }
        }
    }

    /* Convert digit array to a seq_t value */
    seq_t result = 0;
    for (int i = 0; i < L; ++i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    return result;
}
