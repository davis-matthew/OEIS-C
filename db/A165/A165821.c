/*
 * A165821  a(n) = the smallest positive integer that, when written in binary,
 *          contains both binary n and the binary representation of the n-th
 *          prime as substrings.
 * https://oeis.org/A165821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binstr_to_seq, get_prime, merge_patterns
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search implementation of A165821:
   a(n) = smallest positive integer whose binary representation contains
   both binary n and binary nth prime as substrings. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/binstr_to_seq.h"
#include "../utils/merge_patterns.h"

static size_t idx_to_bin(idx_t v, char *buf) {
    char tmp[65];
    int pos = 0;
    do {
        tmp[pos++] = (char)('0' + (v & 1));
        v >>= 1;
    } while (v);
    for (int i = 0; i < pos; ++i) buf[i] = tmp[pos - 1 - i];
    buf[pos] = '\0';
    return (size_t)pos;
}

static bool contains_substr(const char *hay, const char *needle) {
    size_t hlen = strlen(hay);
    size_t nlen = strlen(needle);
    if (nlen > hlen) return false;
    for (size_t i = 0; i <= hlen - nlen; ++i) {
        if (strncmp(hay + i, needle, nlen) == 0) return true;
    }
    return false;
}

static size_t max_overlap(const char *a, size_t alen, const char *b, size_t blen) {
    size_t maxk = (alen < blen) ? alen : blen;
    for (size_t k = maxk; k > 0; --k) {
        bool ok = true;
        for (size_t i = 0; i < k; ++i) {
            if (a[alen - k + i] != b[i]) {
                ok = false;
                break;
            }
        }
        if (ok) return k;
    }
    return 0;
}

seq_t a(idx_t n) {
    /* Convert n and the nth prime to binary strings. */
    char n_bin[65];
    char p_bin[65];
    size_t n_len = idx_to_bin(n, n_bin);
    uint32_t nth_prime = get_prime(n);   /* provided by utils */
    size_t p_len = idx_to_bin((idx_t)nth_prime, p_bin);

    /* If one string contains the other, that string is minimal. */
    if (contains_substr(n_bin, p_bin)) return binstr_to_seq(n_bin, n_len);
    if (contains_substr(p_bin, n_bin)) return binstr_to_seq(p_bin, p_len);

    /* Try the two possible overlaps. */
    size_t ov_n_p = max_overlap(n_bin, n_len, p_bin, p_len);   /* n then p */
    size_t ov_p_n = max_overlap(p_bin, p_len, n_bin, n_len);   /* p then n */

    char merged1[131];
    char merged2[131];
    size_t len1, len2;
    merge_patterns(n_bin, n_len, p_bin, p_len, ov_n_p, merged1, &len1);
    merge_patterns(p_bin, p_len, n_bin, n_len, ov_p_n, merged2, &len2);

    seq_t val1 = binstr_to_seq(merged1, len1);
    seq_t val2 = binstr_to_seq(merged2, len2);

    /* Choose the shorter; if equal, choose the smaller numeric value. */
    if (len1 < len2) return val1;
    if (len2 < len1) return val2;
    return (val1 < val2) ? val1 : val2;
}
