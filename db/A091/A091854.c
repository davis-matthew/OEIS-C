/*
 * A091854  Index of first occurrence of n in A091853, or 0 if no such number
 *          exists.
 * https://oeis.org/A091854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least k >= 1 with A091853(k) = n (search, memoized);
 *            A091853(k) = number of digit permutations (leading zeros
 *            allowed) of k with gcd(k, perm) > 1
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A091854_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void A091854_rec(int pos, int len, uint64_t curr, uint64_t n,
                        uint64_t *cnt, int *freq) {
    if (pos == len) {
        if (A091854_gcd(n, curr) > 1) (*cnt)++;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (freq[d]) {
            freq[d]--;
            A091854_rec(pos + 1, len, curr * 10 + d, n, cnt, freq);
            freq[d]++;
        }
    }
}

static uint64_t A091854_count(uint64_t n) {
    int freq[10] = {0};
    int len = 0;
    uint64_t t = n;
    while (t) { freq[t % 10]++; t /= 10; len++; }
    uint64_t c = 0;
    A091854_rec(0, len, 0, n, &c, freq);
    return c;
}

#define A091854_MAX 256

seq_t a(idx_t n) {
    static uint64_t first[A091854_MAX];
    static bool found[A091854_MAX];
    static uint64_t next_k = 1;
    if (n >= A091854_MAX) return 0;
    while (!found[n]) {
        uint64_t k = next_k++;
        uint64_t c = A091854_count(k);
        if (c < A091854_MAX && !found[c]) {
            found[c] = true;
            first[c] = k;
        }
    }
    return (seq_t)first[n];
}
