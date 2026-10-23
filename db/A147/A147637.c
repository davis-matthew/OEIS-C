/*
 * A147637  a(n) = largest n-distinct-decimal-digit number such that the
 *          string formed by the last k digits is divisible by k for any
 *          1<=k<=n.
 * https://oeis.org/A147637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Computes the largest n‑digit number with distinct decimal digits such that
   the suffix of length k is a multiple of k for all 1 ≤ k ≤ n.
   All values are computed by exhaustive search over the 10! permutations of the digits
   0–9 (n ≤ 10). The results are memoised; for n > 10 the function returns 0. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Generates the next lexicographic permutation of a[0..n-1] in place.
   Returns false if there is no further permutation. */
static bool next_perm(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (a[i] > a[j]) --j;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t cache[11];
    static bool ready = false;
    if (!ready) {
        static uint64_t pow10[11];
        pow10[0] = 1ULL;
        for (int i = 1; i <= 10; ++i) pow10[i] = pow10[i - 1] * 10ULL;

        uint64_t maxv[11] = {0};

        int digits[10];
        for (int i = 0; i < 10; ++i) digits[i] = i;

        do {
            if (digits[0] == 0) continue;          /* leading zero would not form an n‑digit number */
            uint64_t val = 0;
            for (int len = 1; len <= 10; ++len) {
                val = val * 10 + (uint64_t)digits[len - 1];
                bool ok = true;
                for (int k = 1; k <= len; ++k) {
                    uint64_t suffix = val % pow10[k];
                    if (suffix % (uint64_t)k != 0) { ok = false; break; }
                }
                if (ok && val > maxv[len]) maxv[len] = val;
            }
        } while (next_perm(digits, 10));

        for (int i = 1; i <= 10; ++i) cache[i] = (seq_t)maxv[i];
        ready = true;
    }

    if (n < 1 || n > 10) return (seq_t)0;
    return cache[(size_t)n];
}
