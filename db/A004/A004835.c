/*
 * A004835  Numbers that are the sum of at most 6 nonzero 4th powers.
 * https://oeis.org/A004835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve. cnt(v) = min over fourth powers p <= v of cnt(v-p)+1,
 *            with cnt(0) = 0; a(n) is the n-th v >= 0 with cnt(v) <= 6 (the
 *            limit doubles until there are enough terms)
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned long long *A004835_terms = NULL;
static size_t A004835_nterms = 0;
static unsigned long long A004835_lim = 0;

static void A004835_build(unsigned long long L)
{
    unsigned char *cnt = (unsigned char *)malloc((size_t)(L + 1));
    unsigned long long fp[256];
    int nfp = 0;
    unsigned long long b, v;
    size_t k = 0;

    for (b = 1; b * b * b * b <= L && nfp < 256; b++)
        fp[nfp++] = b * b * b * b;

    if (A004835_terms) free(A004835_terms);
    A004835_terms = (unsigned long long *)malloc((size_t)(L + 1) * sizeof(unsigned long long));

    cnt[0] = 0;
    A004835_terms[k++] = 0;
    for (v = 1; v <= L; v++) {
        unsigned char best = 7;
        int i;
        for (i = 0; i < nfp && fp[i] <= v; i++) {
            unsigned char c = (unsigned char)(cnt[v - fp[i]] + 1);
            if (c < best) best = c;
        }
        cnt[v] = best;
        if (best <= 6) A004835_terms[k++] = v;
    }
    A004835_nterms = k;
    free(cnt);
}

seq_t a(idx_t n)
{
    while (A004835_nterms < (size_t)n) {
        A004835_lim = A004835_lim ? A004835_lim * 2 : 4096;
        A004835_build(A004835_lim);
    }
    return (seq_t)A004835_terms[n - 1];
}
