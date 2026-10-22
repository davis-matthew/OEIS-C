/*
 * A007366  Numbers k such that phi(x) = k has exactly 2 solutions.
 * https://oeis.org/A007366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve phi(x) for x <= 7*K (x/phi(x) < 7 for x < 6.5e9, so every
 *            preimage of k <= K is covered), count preimages of each k <= K,
 *            and list the k with exactly 2 solutions; K doubles until there
 *            are n terms
 * Range:     u8 n <= 34, u16 n <= 5227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t curK = 0;

static void build(uint64_t K) {
    uint64_t X = 7 * K;
    uint32_t *ph = (uint32_t *)malloc((size_t)(X + 1) * sizeof(uint32_t));
    unsigned char *cnt = (unsigned char *)calloc((size_t)(K + 1), 1);
    uint64_t i, m, c;
    for (i = 0; i <= X; i++) ph[i] = (uint32_t)i;
    for (i = 2; i <= X; i++) {
        if (ph[i] == i) {
            for (m = i; m <= X; m += i) ph[m] -= ph[m] / (uint32_t)i;
        }
    }
    for (i = 1; i <= X; i++) {
        uint32_t v = ph[i];
        if (v <= K && cnt[v] < 3) cnt[v]++;
    }
    free(ph);
    c = 0;
    for (i = 1; i <= K; i++) if (cnt[i] == 2) c++;
    free(terms);
    terms = (uint64_t *)malloc((size_t)(c + 1) * sizeof(uint64_t));
    nterms = 0;
    for (i = 1; i <= K; i++) if (cnt[i] == 2) terms[nterms++] = i;
    free(cnt);
    curK = K;
}

seq_t a(idx_t n) {
    while (nterms < n) {
        uint64_t K = curK ? curK * 2 : ((uint64_t)1 << 16);
        build(K);
    }
    return (seq_t)terms[n - 1];
}
