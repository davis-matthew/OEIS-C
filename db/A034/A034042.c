/*
 * A034042  Numbers that are primitively or imprimitively represented by
 *          (x^2+xy+2y^2, x>=0, y>=0), but not both.
 * https://oeis.org/A034042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   generate all numbers of the form x^2 + x*y + 2*y^2 with x>=0,
 *            y>=0, track whether they have a primitive (gcd(x,y)=1) or an
 *            imprimitive representation (gcd(x,y)>1). Return the nth integer
 *            that has exactly one type of representation (primitive xor
 *            imprimitive). The function precomputes the sequence...
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd64.h"

static bool *prim = NULL;
static bool *impr = NULL;
static seq_t *terms = NULL;
static size_t terms_len = 0;

/* maximal number to sieve; chosen large enough to contain >10⁴ terms */
#define SIEVE_MAX 2000000

static void precompute_sequence(void) {
    if (terms_len) return;   /* already done */

    const idx_t B = SIEVE_MAX;
    prim = (bool*)malloc((B + 1) * sizeof(bool));
    impr = (bool*)malloc((B + 1) * sizeof(bool));
    if (!prim || !impr) exit(1);
    memset(prim, 0, (B + 1) * sizeof(bool));
    memset(impr, 0, (B + 1) * sizeof(bool));

    for (uint64_t x = 0; x * x <= B; ++x) {
        for (uint64_t y = 0;; ++y) {
            uint64_t val = x * x + x * y + 2 * y * y;
            if (val > B) break;
            if (val == 0) continue;     /* skip 0 */
            uint64_t g = gcd64(x, y);
            if (g == 0) continue;       /* should not happen */
            if (g == 1)
                prim[val] = 1;
            else
                impr[val] = 1;
        }
    }

    /* allocate array for terms; we'll grow as needed */
    size_t cap = 0;
    terms_len = 0;
    terms = NULL;
    for (uint64_t i = 1; i <= B; ++i) {
        if (prim[i] ^ impr[i]) {          /* exactly one type of representation */
            if (terms_len == cap) {
                cap = (cap == 0) ? 1024 : cap * 2;
                seq_t *tmp = (seq_t*)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of range, though offset starts at 1 */
    precompute_sequence();
    if ((idx_t)terms_len < n) return 0;   /* requested term beyond precomputed */
    return terms[n - 1];
}
