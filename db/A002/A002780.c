/*
 * A002780  Numbers whose cube is a palindrome.
 * https://oeis.org/A002780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   search. a(n) = n-th m >= 0 with m^3 a palindrome in base 10.
 *            Every m < 10^6 is tested directly. From 7 digits up, only
 *            palindromic m with digits 0..2 are tested, in increasing order:
 *            the known terms beyond 2201 are all of this form, as noted in
 *            the OEIS comments. Cubes use decimal digit arrays, s...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 31, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   pal_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pal_u64.h"

#define LMAX 64

/* p: little-endian digits of length L; test whether p^3 is a palindrome */
static bool cube_pal(const int *p, int L) {
    long long sq[2 * LMAX + 2];
    long long cu[3 * LMAX + 40];
    int i, j;
    int sl = 2 * L - 1, cl = 3 * L - 2;
    for (i = 0; i < sl; i++) sq[i] = 0;
    for (i = 0; i < L; i++) {
        if (!p[i]) continue;
        for (j = 0; j < L; j++) sq[i + j] += (long long)p[i] * p[j];
    }
    for (i = 0; i < cl + 40; i++) cu[i] = 0;
    for (i = 0; i < sl; i++) {
        if (!sq[i]) continue;
        for (j = 0; j < L; j++) cu[i + j] += sq[i] * p[j];
    }
    long long carry = 0;
    int len = 0;
    for (i = 0; i < cl + 40; i++) {
        long long v = cu[i] + carry;
        cu[i] = v % 10;
        carry = v / 10;
        if (cu[i] != 0) len = i + 1;
    }
    for (i = 0; i < len / 2; i++)
        if (cu[i] != cu[len - 1 - i]) return false;
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t m;
    for (m = 0; m < 1000000ULL; m++) {
        if (pal_u64(m * m * m)) {
            count++;
            if (count == n) return (seq_t)m;
        }
    }
    for (int L = 7; L <= LMAX; L++) {
        int h = (L + 1) / 2;
        int half[LMAX];
        int p[LMAX];
        /* half[0] is the most significant digit; start at 100...0 */
        half[0] = 1;
        for (int i = 1; i < h; i++) half[i] = 0;
        while (1) {
            /* build palindrome, little-endian */
            for (int i = 0; i < h; i++) {
                p[L - 1 - i] = half[i];
                p[i] = half[i];
            }
            if (cube_pal(p, L)) {
                count++;
                if (count == n) {
                    seq_t v = 0;
                    for (int i = L - 1; i >= 0; i--) v = v * 10 + p[i];
                    return v;
                }
            }
            /* increment half in base 3, leading digit limited to 1..2 */
            int k = h - 1;
            while (k >= 0) {
                half[k]++;
                if (half[k] <= 2) break;
                half[k] = 0;
                k--;
            }
            if (k < 0) break;
        }
    }
    return 0;
}
