/*
 * A091460  Greatest size of an arithmetic progression contained as substrings
 *          in binary representation of n.
 * https://oeis.org/A091460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum length of an arithmetic progression whose terms *
 *            appear as contiguous substrings in the binary representation of
 *            n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Special case: n = 0 has binary representation "0". */
    if (n == 0) {
        return (seq_t)1;   /* only the substring "0" is present */
    }

    /* Determine the bit length L of n. */
    int L = 0;
    uint64_t temp = n;
    while (temp) {
        ++L;
        temp >>= 1;
    }

    /* Maximum value that can appear as a substring.               */
    /* For L bits, the maximum binary value is (1<<L)-1 <= 16383. */
    int max_possible = 1 << L;
    static bool present[1 << 15];   /* 32768 entries are enough */
    memset(present, 0, max_possible * sizeof(bool));

    /* Bits of n from most significant to least significant. */
    int bits[64];
    for (int i = 0; i < L; ++i) {
        bits[i] = (n >> (L - 1 - i)) & 1;
    }

    /* Mark all substrings as present. */
    for (int i = 0; i < L; ++i) {
        int val = 0;
        for (int j = i; j < L; ++j) {
            val = (val << 1) | bits[j];
            present[val] = true;
        }
    }

    /* Collect present values into a small list. */
    int vals[200];
    int cnt = 0;
    for (int v = 0; v < max_possible; ++v) {
        if (present[v]) {
            vals[cnt++] = v;
        }
    }

    seq_t max_len = 1;   /* at least one substring is always present */

    for (int i = 0; i < cnt; ++i) {
        for (int j = i + 1; j < cnt; ++j) {
            int a = vals[i];
            int d = vals[j] - vals[i];   /* positive difference */
            if (d <= 0) continue;
            int x = a;
            int cur = 0;
            while (x < max_possible && present[x]) {
                ++cur;
                x += d;
            }
            if (cur > max_len) {
                max_len = cur;
            }
        }
    }

    return max_len;
}
