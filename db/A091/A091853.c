/*
 * A091853  a(n) is the number of numbers k such that k is obtained by
 *          permuting the digits of n and gcd(n,k) > 1.
 * https://oeis.org/A091853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = number of distinct permutations of the decimal digits of
 *            n that are coprime to n (i.e., have gcd(n,k) > 1)
 * Range:     u8 n <= 104, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t (any unsigned integer type). */
static seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Recursive enumeration of distinct permutations using digit frequencies. */
static void rec(int pos, int len, seq_t curr, seq_t n, uint64_t *cnt,
                uint8_t *freq) {
    if (pos == len) {
        if (gcd_seq_t(n, curr) > 1)
            (*cnt)++;
        return;
    }
    for (int d = 0; d < 10; ++d) {
        if (freq[d] != 0) {
            freq[d]--;
            rec(pos + 1, len, curr * 10 + d, n, cnt, freq);
            freq[d]++;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0)      /* offset starts at 1, but handle 0 defensively */
        return 0;

    uint8_t freq[10] = {0};
    idx_t tmp = n;
    int len = 0;

    /* Extract decimal digits and count frequencies. */
    while (tmp > 0) {
        int d = tmp % 10;
        freq[d]++;
        tmp /= 10;
        len++;
    }

    uint64_t count = 0;
    rec(0, len, (seq_t)0, (seq_t)n, &count, freq);

    return (seq_t)count;
}
