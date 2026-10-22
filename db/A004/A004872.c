/*
 * A004872  Numbers that are the sum of at most 10 positive 7th powers.
 * https://oeis.org/A004872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums of at most 10 positive 7th powers
 *            up to a bound (bitset), list them in increasing order
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *A004872_terms = NULL;
static uint64_t A004872_count = 0;
static uint64_t A004872_limit = 0;

static uint64_t A004872_pw[64];
static int A004872_npw;
static unsigned char *A004872_bits;

static void A004872_dfs(uint64_t sum, int maxb, int rem, uint64_t L) {
    A004872_bits[sum >> 3] |= (unsigned char)(1u << (sum & 7));
    if (rem == 0) return;
    for (int b = 1; b <= maxb; b++) {
        uint64_t p = A004872_pw[b];
        if (p > L - sum) break;
        A004872_dfs(sum + p, b, rem - 1, L);
    }
}

static void A004872_build(uint64_t L) {
    A004872_npw = 0;
    for (int b = 1; b < 64; b++) {
        uint64_t p = 1;
        int over = 0;
        for (int k = 0; k < 7; k++) {
            if (p > L / (uint64_t)b) { over = 1; break; }
            p *= (uint64_t)b;
        }
        if (over || p > L) break;
        A004872_pw[b] = p;
        A004872_npw = b;
    }
    size_t nbytes = (size_t)(L / 8 + 1);
    A004872_bits = (unsigned char *)calloc(nbytes, 1);
    A004872_dfs(0, A004872_npw, 10, L);

    uint64_t cnt = 0;
    for (uint64_t i = 0; i <= L; i++)
        if (A004872_bits[i >> 3] & (1u << (i & 7))) cnt++;

    free(A004872_terms);
    A004872_terms = (uint64_t *)malloc((size_t)(cnt ? cnt : 1) * sizeof(uint64_t));
    uint64_t j = 0;
    for (uint64_t i = 0; i <= L; i++)
        if (A004872_bits[i >> 3] & (1u << (i & 7))) A004872_terms[j++] = i;
    free(A004872_bits);
    A004872_bits = NULL;
    A004872_count = cnt;
    A004872_limit = L;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (A004872_terms == NULL || n > A004872_count) {
        uint64_t L = A004872_limit ? A004872_limit : 4096;
        do {
            L *= 2;
            A004872_build(L);
        } while (n > A004872_count);
    }
    return (seq_t)A004872_terms[n - 1];
}
