/*
 * A001575  Colored series-parallel networks.
 * https://oeis.org/A001575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = N_3(n) - 3*N_2(n) + 3*N_1(n), where N_c(n) is the number
 *            of series-parallel networks with n edges colored from c colors
 *            (so a(n) counts those using all 3 colors). N_c(1) = c, N_c(n) =
 *            2*s_c(n) for n >= 2, where U = c*x + S and 1 + U + S = Euler
 *            transform of U, i.e. n*(u_n + s_n) = sum_{k=1...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   from_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/from_u128.h"

#define DEF_EXT(NAME, T)                                                     \
static void NAME(T **u, T **m, T **cc, idx_t *len, idx_t *cap, idx_t n) {   \
    idx_t i, k, d;                                                           \
    int c;                                                                   \
    if (n + 1 > *cap) {                                                      \
        idx_t nc = (*cap) * 2;                                               \
        if (nc < n + 1) nc = n + 1;                                          \
        for (c = 0; c < 3; c++) {                                            \
            T *nu = (T *)malloc(nc * sizeof(T));                             \
            T *nm = (T *)malloc(nc * sizeof(T));                             \
            T *ncc = (T *)malloc(nc * sizeof(T));                            \
            for (i = 0; i < nc; i++) { nu[i] = 0; nm[i] = 0; ncc[i] = 0; }   \
            for (i = 0; i < *len; i++) {                                     \
                nu[i] = u[c][i]; nm[i] = m[c][i]; ncc[i] = cc[c][i];         \
            }                                                                \
            if (u[c]) { free(u[c]); free(m[c]); free(cc[c]); }               \
            u[c] = nu; m[c] = nm; cc[c] = ncc;                               \
        }                                                                    \
        *cap = nc;                                                           \
    }                                                                        \
    for (i = (*len < 1 ? 1 : *len); i <= n; i++) {                           \
        for (c = 0; c < 3; c++) {                                            \
            T *U = u[c], *M = m[c], *C = cc[c];                              \
            if (i == 1) {                                                    \
                T col = (T)(idx_t)(c + 1);                                   \
                U[1] = col; M[1] = col; C[1] = col;                          \
            } else {                                                         \
                T cd = 0;                                                    \
                for (d = 1; d <= i / 2; d++)                                 \
                    if (i % d == 0) cd += (T)d * U[d];                       \
                T rest = cd;                                                 \
                for (k = 1; k < i; k++) rest += C[k] * M[i - k];             \
                T s = rest / (T)i;                                           \
                U[i] = s;                                                    \
                M[i] = s + s;                                                \
                C[i] = cd + (T)i * s;                                        \
            }                                                                \
        }                                                                    \
    }                                                                        \
    if (*len < n + 1) *len = n + 1;                                          \
}

DEF_EXT(ext_big, seq_t)
DEF_EXT(ext_fix, unsigned __int128)

seq_t a(idx_t n) {
    static int mode = -1;
    if (mode < 0) {
        seq_t t = 1;
        int i;
        for (i = 0; i < 200; i++) t *= 2;
        mode = (t == 0) ? 1 : 0;
    }
    if (n < 1) return 0;
    if (mode == 1) {
        static unsigned __int128 *u[3] = {NULL, NULL, NULL};
        static unsigned __int128 *m[3] = {NULL, NULL, NULL};
        static unsigned __int128 *cc[3] = {NULL, NULL, NULL};
        static idx_t len = 0, cap = 0;
        ext_fix(u, m, cc, &len, &cap, n);
        unsigned __int128 r = m[2][n] - 3 * m[1][n] + 3 * m[0][n];
        return from_u128(r);
    } else {
        static seq_t *u[3] = {NULL, NULL, NULL};
        static seq_t *m[3] = {NULL, NULL, NULL};
        static seq_t *cc[3] = {NULL, NULL, NULL};
        static idx_t len = 0, cap = 0;
        ext_big(u, m, cc, &len, &cap, n);
        seq_t r = m[2][n] + 3 * m[0][n];
        r -= 3 * m[1][n];
        return r;
    }
}
