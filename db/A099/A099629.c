/*
 * A099629  Smallest and largest primes pairwise displayed with k digits from
 *          k=1,...,9 with distinct decimal digits.
 * https://oeis.org/A099629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = smallest (odd n) or largest (even n) prime with k
 *            distinct decimal digits where k = (n+1)/2 for odd n and k = n/2
 *            for even n (k = 1 … 9). A(n) = 0 for n not in this range
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Recursive search for the smallest prime of length k (ascending order). */
static void find_smallest(int pos, int k, uint16_t used, idx_t cur,
                          idx_t *best, bool *found)
{
    if (*found) return;
    if (pos == k) {
        if (is_prime(cur)) {
            *best = cur;
            *found = true;
        }
        return;
    }
    int start = (pos == 0) ? 1 : 0;           /* leading digit cannot be 0 */
    for (int d = start; d <= 9; ++d) {
        if (!(used & (1U << d))) {
            find_smallest(pos + 1, k, used | (1U << d), cur * 10 + d,
                          best, found);
            if (*found) return;
        }
    }
}

/* Recursive search for the largest prime of length k (descending order). */
static void find_largest(int pos, int k, uint16_t used, idx_t cur,
                         idx_t *best, bool *found)
{
    if (*found) return;
    if (pos == k) {
        if (is_prime(cur)) {
            *best = cur;
            *found = true;
        }
        return;
    }
    int start = (pos == 0) ? 9 : 9;           /* leading digit cannot be 0 */
    for (int d = start; d >= 0; --d) {
        if (pos == 0 && d == 0) continue;     /* skip leading 0 */
        if (!(used & (1U << d))) {
            find_largest(pos + 1, k, used | (1U << d), cur * 10 + d,
                         best, found);
            if (*found) return;
        }
    }
}

/* Cache for smallest and largest primes for digit lengths 1‑9. */
static seq_t smallest_prime[10];
static seq_t largest_prime[10];
static bool ready_small[10] = {false};
static bool ready_large[10] = {false};

/* Compute smallest prime for a given digit length k (1 ≤ k ≤ 9). */
static void compute_smallest(int k)
{
    idx_t val = 0;
    bool found = false;
    find_smallest(0, k, 0, 0, &val, &found);
    if (found)
        smallest_prime[k] = (seq_t)val;
    else
        smallest_prime[k] = 0;
    ready_small[k] = true;
}

/* Compute largest prime for a given digit length k (1 ≤ k ≤ 9). */
static void compute_largest(int k)
{
    idx_t val = 0;
    bool found = false;
    find_largest(0, k, 0, 0, &val, &found);
    if (found)
        largest_prime[k] = (seq_t)val;
    else
        largest_prime[k] = 0;
    ready_large[k] = true;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Offset is 1; for n < 1 we return 0. */
    if (n < 1) return 0;

    /* Determine the digit length k. */
    int k;
    if (n & 1) {                /* odd n → smallest prime */
        k = (int)((n + 1) / 2);
        if (k < 1 || k > 9) return 0;
        if (!ready_small[k]) compute_smallest(k);
        return smallest_prime[k];
    } else {                    /* even n → largest prime */
        k = (int)(n / 2);
        if (k < 1 || k > 9) return 0;
        if (!ready_large[k]) compute_largest(k);
        return largest_prime[k];
    }
}
