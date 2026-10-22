/*
 * A003978  Möbius transform of A003964.
 * https://oeis.org/A003978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(p^e) = (q-1)*q^(e-1), q =
 *            A000041(A000720(p)+1)
 * Range:     u8 n <= 52, u16 n <= 190, u32 n <= 708, u64 n <= 2860,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *part_tab = 0;
static idx_t part_len = 0;

static uint32_t *pi_tab = 0;
static idx_t pi_len = 0;

static void ensure_pi(idx_t m) {
    if (m < pi_len) return;
    idx_t len = pi_len ? pi_len : 1024;
    while (len <= m) len *= 2;
    uint32_t *t = (uint32_t *)malloc(len * sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc(len, 1);
    for (idx_t i = 2; i * i < len; i++)
        if (!comp[i])
            for (idx_t j = i * i; j < len; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (idx_t i = 0; i < len; i++) {
        if (i >= 2 && !comp[i]) c++;
        t[i] = c;
    }
    free(comp);
    free(pi_tab);
    pi_tab = t;
    pi_len = len;
}

static seq_t partition(idx_t k) {
    if (k >= part_len) {
        idx_t len = part_len ? part_len : 256;
        while (len <= k) len *= 2;
        seq_t *p = alloc_zero(len);
        p[0] = 1;
        for (idx_t i = 1; i < len; i++)
            for (idx_t j = i; j < len; j++)
                p[j] += p[j - i];
        if (part_tab) release(part_tab, part_len);
        part_tab = p;
        part_len = len;
    }
    return part_tab[k];
}

static seq_t prime_factor(idx_t p, idx_t e) {
    ensure_pi(p);
    seq_t q = partition((idx_t)pi_tab[p] + 1);
    seq_t r = q - 1;
    for (idx_t i = 1; i < e; i++) r *= q;
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            result *= prime_factor(p, e);
        }
    }
    if (temp > 1) result *= prime_factor(temp, 1);
    return result;
}
