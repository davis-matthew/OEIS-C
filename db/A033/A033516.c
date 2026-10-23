/*
 * A033516  Number of matchings in graph C_{4} X P_{n}.
 * https://oeis.org/A033516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (M^n)[0][0] – the (0,0) entry of the n‑th power of the
 *            transfer matrix M for the graph C₄ × Pₙ. The matrix counts how a
 *            column of the product graph (a 4‑cycle) can be matched given the
 *            vertical edges on its left and right sides
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static seq_t M[16][16];
static bool initialized = false;

static void init_matrix(void) {
    if (initialized) return;
    const uint8_t h_masks[7] = {0x0, 0x3, 0x6, 0xC, 0x9, 0xF, 0xF};
    for (int s = 0; s < 16; ++s) {
        for (int t = 0; t < 16; ++t) M[s][t] = (seq_t)0;
        for (int hm = 0; hm < 7; ++hm) {
            uint8_t hm_mask = h_masks[hm];
            if (hm_mask & s) continue;                /* left match conflict */
            for (uint8_t vm = 0; vm < 16; ++vm) {
                if ((vm & s) != 0) continue;           /* left match conflict */
                if ((hm_mask & vm) != 0) continue;      /* vertex used twice */
                int t = vm;                             /* right match mask */
                M[s][t] += (seq_t)1;
            }
        }
    }
    initialized = true;
}

static void mat_mul(const seq_t A[16][16], const seq_t B[16][16],
                    seq_t C[16][16]) {
    seq_t tmp[16][16] = {0};
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < 16; ++k) {
                if (A[i][k] == (seq_t)0 || B[k][j] == (seq_t)0) continue;
                sum += A[i][k] * B[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            C[i][j] = tmp[i][j];
}

seq_t a(idx_t n) {
    init_matrix();
    if (n == 0) return (seq_t)1;

    seq_t base[16][16];
    seq_t result[16][16];
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            base[i][j] = M[i][j];
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    while (n > 0) {
        if (n & 1) {
            seq_t temp[16][16];
            mat_mul(result, base, temp);
            for (int i = 0; i < 16; ++i)
                for (int j = 0; j < 16; ++j)
                    result[i][j] = temp[i][j];
        }
        seq_t temp[16][16];
        mat_mul(base, base, temp);
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                base[i][j] = temp[i][j];
        n >>= 1;
    }

    return result[0][0];
}
