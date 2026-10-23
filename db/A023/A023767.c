/*
 * A023767  Nialpdromes: digits in base 12 are in nonincreasing order.
 * https://oeis.org/A023767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023767 – Nialpdromes: digits in base‑12 are in non‑increasing order.
   The function returns the n‑th such integer in increasing order. */

#include <stdint.h>

/* binomial coefficient C(n,k) for small n (fits in 64 bits). */
static inline uint64_t binom(uint64_t n, uint64_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t r = 1;
    for (uint64_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* Construct the k‑digit nialpdrome that is the idx‑th (1‑based) among all k‑digit
   nialpdromes (indexed lexicographically by the digits). */
static inline seq_t construct_k(idx_t k, idx_t idx)
{
    idx_t prev_digit = 11;
    seq_t value = 0;
    for (idx_t pos = 0; pos < k; ++pos) {
        idx_t remaining = k - pos - 1;
        int lower = (pos == 0) ? 1 : 0;
        for (int d = lower; d <= (int)prev_digit; ++d) {
            uint64_t cnt = binom((uint64_t)d + remaining, remaining);
            if (idx <= cnt) {
                value = value * (seq_t)12 + (seq_t)d;
                prev_digit = d;
                break;
            } else {
                idx -= cnt;
            }
        }
    }
    return value;
}

seq_t a(idx_t n)
{
    /* Handle the first 12 trivial cases: 0 through 11. */
    if (n <= 12) return (seq_t)(n - 1);

    /* n > 12: skip the 12 single‑digit numbers. */
    idx_t idx = n - 12;

    for (idx_t k = 2;; ++k) {
        /* Count how many k‑digit nialpdromes exist. */
        uint64_t count_k = 0;
        for (idx_t d = 1; d <= 11; ++d)
            count_k += binom(d + k - 1, k - 1);

        if (idx <= count_k)
            return construct_k(k, idx);
        idx -= count_k;
    }
}
