/*
 * A122425  a(n) = lexicographically greatest divisor of n in decimal
 *          representation.
 * https://oeis.org/A122425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Search: for a given n, examine all its divisors and return the
   divisor that is lexicographically greatest when written in base
   10.  */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    for (idx_t i = 1; i <= n / i; ++i) r = i;
    return r;
}

/* Convert a seq_t value to a decimal string.  The string is written
   into the supplied buffer and *len receives its length. */
static void num_to_str(seq_t x, char *out, int *len) {
    int pos = 0;
    if (x == 0) {
        out[pos++] = '0';
    } else {
        while (x > 0) {
            int d = (int)(x % 10);
            out[pos++] = '0' + d;
            x /= 10;
        }
        /* reverse */
        for (int i = 0; i < pos / 2; ++i) {
            char t = out[i];
            out[i] = out[pos - 1 - i];
            out[pos - 1 - i] = t;
        }
    }
    out[pos] = '\0';
    if (len) *len = pos;
}

/* Lexicographic comparison of two decimal strings.  Returns
   positive if a > b, negative if a < b, zero if equal. */
static int cmp_str(const char *a, int len_a, const char *b, int len_b) {
    int i = 0;
    while (i < len_a && i < len_b) {
        if (a[i] != b[i])
            return (a[i] > b[i]) ? 1 : -1;
        ++i;
    }
    if (len_a != len_b) return (len_a > len_b) ? 1 : -1;
    return 0;
}

/* Try to replace the current best divisor by the candidate d. */
static void try_update(seq_t d,
                       seq_t *best_val,
                       char *best_str,
                       int *best_len) {
    char buf[65];
    int blen;
    num_to_str(d, buf, &blen);
    if (cmp_str(buf, blen, best_str, *best_len) > 0) {
        for (int i = 0; i < blen; ++i) best_str[i] = buf[i];
        best_str[blen] = '\0';
        *best_len = blen;
        *best_val = d;
    }
}

seq_t a(idx_t n) {
    seq_t best_val = 1;
    char best_str[65];
    int  best_len;
    num_to_str(1, best_str, &best_len);

    idx_t sqrt_n = isqrt_u64(n);
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            try_update(d1, &best_val, best_str, &best_len);
            seq_t d2 = (seq_t)(n / i);
            if (d2 != d1)
                try_update(d2, &best_val, best_str, &best_len);
        }
    }
    return best_val;
}
