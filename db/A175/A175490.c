/*
 * A175490  a(n) = Product_{k=1..n} b(k,n), where b(k,n) is the largest
 *          positive integer that, when written in binary, occurs as a
 *          substring in both binary k and binary n.
 * https://oeis.org/A175490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ∏_{k=1..n} b(k,n) where * b(k,n) = the largest positive
 *            integer whose binary representation * is a substring of both
 *            binary(k) and binary(n)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 18, u64 n <= 28, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: determine bit length of an idx_t value. */
static int bitlen_idx(idx_t v)
{
    int len = 0;
    while (v) { ++len; v >>= 1; }
    return len ? len : 1;        /* for v==0 (not used here) */
}

/* Helper: build binary string (MSB first) for an idx_t value. */
static char *build_bin(idx_t v, int *plen)
{
    int len = bitlen_idx(v);
    *plen = len;
    char *s = (char *)malloc((size_t)len + 1);
    for (int i = 0; i < len; ++i)  /* i: position from MSB */
        s[i] = ((v >> (len - 1 - i)) & 1) ? '1' : '0';
    s[len] = '\0';
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but safe */
    int len_n, len_k;
    char *s_n = build_bin(n, &len_n);

    /* allocate arrays of bitsets for n's substrings */
    unsigned char **present = (unsigned char **)malloc((size_t)(len_n + 1) * sizeof(unsigned char *));
    for (int L = 1; L <= len_n; ++L) {
        size_t bits = (size_t)1 << L;
        size_t bytes = (bits + 7) >> 3;
        present[L] = (unsigned char *)calloc(bytes, 1);  /* all zero */
    }

    /* set bits for all substrings of n */
    for (int i = 0; i < len_n; ++i) {
        int val = 0;
        for (int j = i; j < len_n; ++j) {
            val = (val << 1) | (s_n[j] - '0');
            int L = j - i + 1;
            present[L][val >> 3] |= (unsigned char)(1 << (val & 7));
        }
    }

    seq_t result = 1;
    char *s_k = NULL;
    for (idx_t k = 1; k <= n; ++k) {
        s_k = build_bin(k, &len_k);
        seq_t b_val = 1;  /* minimal possible value */
        for (int i = 0; i < len_k; ++i) {
            int val = 0;
            for (int j = i; j < len_k; ++j) {
                val = (val << 1) | (s_k[j] - '0');
                int L = j - i + 1;
                /* check presence in n's substrings */
                if (present[L][val >> 3] & (1 << (val & 7))) {
                    if (val > b_val) b_val = val;
                }
            }
        }
        result *= b_val;
        free(s_k);
    }

    /* free allocated structures */
    for (int L = 1; L <= len_n; ++L)
        free(present[L]);
    free(present);
    free(s_n);

    return result;
}
