/*
 * A151713  Weight distribution of [63,10,27] primitive binary BCH code.
 * https://oeis.org/A151713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   gf2_6_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A151713: weight distribution of the primitive binary BCH
 * code [63,10,27].
 * 
 * a(n) = number of codewords of weight n in that code.
 * Implementation:  Build the parity‑check matrix from the roots
 * alpha^i (1 <= i <= 26) over GF(2^6) (primitive element alpha=0x02
 * with irreducible polynomial x^6 + x + 1).  Reduce the matrix over
 * GF(2) to find a generator matrix of rank 10, then enumerate all
 * 2^10 codewords and count their Hamming weights (0 <= n <= 63).
 * For n > 63 the value is 0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gf2_6_mul.h"

static int g_init = 0;
static seq_t a_dist[64] = {0};

static void build_distribution(void) {
    /* pre‑compute powers of alpha (0x02) */
    uint8_t pow[63];
    pow[0] = 1;                     /* alpha^0 = 1 */
    for (int i = 1; i < 63; ++i) pow[i] = gf2_6_mul(pow[i-1], 2);

    /* construct 156 × 63 parity‑check matrix over GF(2) */
    #define H_ROWS 156
    uint64_t H[H_ROWS];
    for (int row = 0; row < H_ROWS; ++row) H[row] = 0;

    int idx = 0;
    for (int i = 1; i <= 26; ++i) {
        for (int k = 0; k < 6; ++k) {
            uint64_t r = 0;
            for (int j = 0; j < 63; ++j) {
                uint8_t val = pow[(i * j) % 63];
                if (val & (1u << k)) r |= 1ull << j;
            }
            H[idx++] = r;            /* idx runs from 0 to 155 */
        }
    }

    /* Gaussian elimination over GF(2) */
    int pivot_of_row[H_ROWS];
    int rank = 0;
    for (int col = 0; col < 63 && rank < H_ROWS; ++col) {
        int r;
        for (r = rank; r < H_ROWS; ++r) {
            if (H[r] & (1ull << col)) break;
        }
        if (r == H_ROWS) continue;   /* no pivot in this column */
        if (r != rank) {
            uint64_t tmp = H[rank];
            H[rank] = H[r];
            H[r] = tmp;
        }
        pivot_of_row[rank] = col;

        for (int r2 = 0; r2 < H_ROWS; ++r2) {
            if (r2 != rank && (H[r2] & (1ull << col))) {
                H[r2] ^= H[rank];
            }
        }
        ++rank;
    }

    /* identify free columns (dimension = 10) */
    int is_pivot[63] = {0};
    for (int r = 0; r < rank; ++r) is_pivot[pivot_of_row[r]] = 1;

    int free_cols[10];
    int free_cnt = 0;
    for (int col = 0; col < 63; ++col) {
        if (!is_pivot[col]) free_cols[free_cnt++] = col;
    }
    /* free_cnt should be 10 */

    /* basis vectors of the code (size 10) */
    uint64_t basis[10];
    for (int f = 0; f < free_cnt; ++f) {
        uint64_t vec = 0;
        int col_f = free_cols[f];
        vec |= 1ull << col_f;               /* free variable is 1 */

        /* compute pivot variable values */
        for (int r = 0; r < rank; ++r) {
            if (H[r] & (1ull << col_f)) {
                vec ^= 1ull << pivot_of_row[r];
            }
        }
        basis[f] = vec;
    }

    /* enumerate all 2^10 codewords and count Hamming weights */
    for (int mask = 0; mask < (1 << 10); ++mask) {
        uint64_t v = 0;
        for (int i = 0; i < 10; ++i)
            if (mask & (1 << i)) v ^= basis[i];
        int w = __builtin_popcountll(v);
        if (w < 64) a_dist[w] += 1;
    }
}

seq_t a(idx_t n) {
    if (!g_init) {
        build_distribution();
        g_init = 1;
    }
    if (n < 64) return a_dist[n];
    return 0;
}
