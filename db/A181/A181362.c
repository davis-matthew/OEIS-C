/*
 * A181362  a(n) is the starting position of the n-th occurrence of n in the
 *          string 123456789101112131415161718192021... .
 * https://oeis.org/A181362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the starting position of the nth occurrence of the
 *            decimal representation of n in the infinite string formed by
 *            concatenating the natural numbers: 1234567891011121314
 * Range:     u8 n <= 11, u16 n <= 149, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation streams the digits of the natural numbers, uses
   a KMP automaton to find occurrences of the pattern (the digits of n),
   and stops when the nth occurrence is found.  It works for all n >= 1
   and for any allowed seq_t type. */

#include <stdint.h>
#include <string.h>

static void int_to_str(uint64_t x, char *buf, int *len) {
    /* Convert x (x>=1) to decimal string in buf, set *len. */
    char tmp[32];
    int pos = 0;
    while (x > 0) {
        tmp[pos++] = (char)('0' + (x % 10));
        x /= 10;
    }
    if (pos == 0) {
        tmp[pos++] = '0';
    }
    for (int i = 0; i < pos; ++i) {
        buf[i] = tmp[pos - 1 - i];
    }
    *len = pos;
}

static void compute_pi(const char *pat, int len, int *pi) {
    pi[0] = 0;
    for (int i = 1; i < len; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j]) {
            j = pi[j - 1];
        }
        if (pat[i] == pat[j]) {
            ++j;
        }
        pi[i] = j;
    }
}

seq_t a(idx_t n) {
    /* n is 1-based. */
    uint64_t target = (uint64_t)n;
    if (target == 0) {
        /* Will never be called with n==0 according to OEIS offset. */
        return (seq_t)0;
    }

    /* Build pattern string for n. */
    char pat[32];
    int pat_len;
    int_to_str(target, pat, &pat_len);

    /* Precompute prefix function for KMP. */
    int pi[32];
    compute_pi(pat, pat_len, pi);

    /* Stream digits of natural numbers. */
    int j = 0;               /* current matched prefix length */
    uint64_t count = 0;      /* number of matches found */
    uint64_t total_pos = 0;  /* position of current digit (1-based) */

    for (uint64_t k = 1; ; ++k) {
        char num_buf[32];
        int num_len;
        int_to_str(k, num_buf, &num_len);

        for (int idx = 0; idx < num_len; ++idx) {
            char c = num_buf[idx];
            ++total_pos;

            while (j > 0 && c != pat[j]) {
                j = pi[j - 1];
            }
            if (c == pat[j]) {
                ++j;
            }
            if (j == pat_len) {
                ++count;
                if (count == target) {
                    /* Starting position of the match. */
                    uint64_t start_pos = total_pos - pat_len + 1;
                    return (seq_t)start_pos;
                }
                /* Prepare for next potential match (overlap). */
                j = pi[j - 1];
            }
        }
    }
}
