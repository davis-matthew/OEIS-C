/*
 * A003339  Numbers that are the sum of 5 positive 4th powers.
 * https://oeis.org/A003339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve. A bitmask knapsack over positive 4th powers, kept up to a
 *            doubling limit, marks every x that is a sum of exactly 5 of
 *            them; a(n) is the n-th marked x
 * Range:     u8 n <= 16, u16 n <= 5719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *a3339_terms = NULL;
static idx_t a3339_count = 0;
static uint64_t a3339_limit = 0;

static void a3339_sieve(uint64_t L) {
    unsigned char *mask = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint64_t b, x, cnt = 0;
    mask[0] = 1;
    for (b = 1; b * b * b * b <= L; b++) {
        uint64_t p = b * b * b * b;
        for (x = 0; x + p <= L; x++) {
            unsigned char m = mask[x];
            if (m) mask[x + p] |= (unsigned char)((m << 1) & 0x3F);
        }
    }
    for (x = 1; x <= L; x++)
        if (mask[x] & 0x20) cnt++;
    free(a3339_terms);
    a3339_terms = (uint64_t *)malloc((size_t)(cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (x = 1; x <= L; x++)
        if (mask[x] & 0x20) a3339_terms[cnt++] = x;
    a3339_count = (idx_t)cnt;
    a3339_limit = L;
    free(mask);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (a3339_limit == 0) a3339_sieve(4096);
    while (a3339_count < n) a3339_sieve(a3339_limit * 2);
    return (seq_t)a3339_terms[n - 1];
}
