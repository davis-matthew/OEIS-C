/*
 * A118388  Least k such that T(k) includes the consecutive digits of n, where
 *          T(k) = k*(k+1)/2.
 * https://oeis.org/A118388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = minimum k such that the decimal representation of n *
 *            occurs as a contiguous substring of T(k) = k*(k+1)/2
 * Range:     u8 n <= 10, u16 n <= 1150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Convert an idx_t to a decimal string, storing it in buf.
 * The string is written without leading zeros, except that 0
 * produces "0".  The caller should provide a buffer of at least
 * 32 bytes, which is plenty for all idx_t values. */
static size_t idx_to_str(idx_t n, char *buf)
{
    if (n == 0) {
        buf[0] = '0';
        return 1;
    }
    size_t len = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        uint64_t d = tmp % 10;
        buf[len++] = (char)('0' + d);
        tmp /= 10;
    }
    /* reverse */
    for (size_t i = 0; i < len / 2; ++i) {
        char c = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = c;
    }
    return len;
}

/* Convert a seq_t to a decimal string, storing it in buf.
 * The buffer should be large enough (at least 200 bytes for typical
 * triangular numbers up to the range needed for n ≤ 10000). */
static size_t seq_to_str(seq_t x, char *buf)
{
    if (x == 0) {
        buf[0] = '0';
        return 1;
    }
    size_t len = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        seq_t d = tmp % 10;
        buf[len++] = (char)('0' + (int)d);
        tmp /= 10;
    }
    /* reverse */
    for (size_t i = 0; i < len / 2; ++i) {
        char c = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = c;
    }
    return len;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Convert n to string once. */
    char target[32];
    size_t tlen = idx_to_str(n, target);

    /* Special case: n==0 returns 0 immediately. */
    if (n == 0)
        return 0;

    /* Iterate over k to find the first triangular number containing n. */
    for (idx_t k = 0; ; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t tri = kseq * (kseq + 1) / 2;

        /* Convert triangular number to string. */
        char tri_str[200];
        size_t tri_len = seq_to_str(tri, tri_str);

        /* Quick rejection if tri is too short. */
        if (tri_len < tlen)
            continue;

        /* Simple substring search. */
        for (size_t i = 0; i <= tri_len - tlen; ++i) {
            size_t j;
            for (j = 0; j < tlen; ++j)
                if (tri_str[i + j] != target[j])
                    break;
            if (j == tlen)
                return (seq_t)k;   /* Found minimal k. */
        }
    }
}
