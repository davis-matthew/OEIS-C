/*
 * A158044  Determinant of power series of gamma matrix with determinant 6!.
 * https://oeis.org/A158044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Determinant(A + A² + … + Aⁿ) where A[i][j] = ((j) % (i +
 *            1)) + 1 for 0 ≤ i,j < 7
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Matrix multiplication: res = a * b */
static void matrix_mul(const seq_t a[7][7], const seq_t b[7][7], seq_t res[7][7]) {
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            seq_t s = 0;
            for (int k = 0; k < 7; ++k) {
                s += a[i][k] * b[k][j];
            }
            res[i][j] = s;
        }
    }
}

/* Generate all 7! permutations and their parity (0 = even, 1 = odd). */
static void gen_permutations(int depth, int used, int perm[7],
                             int perms[5040][7], char parity[5040], size_t *cnt) {
    if (depth == 7) {
        /* store permutation */
        for (int i = 0; i < 7; ++i) perms[*cnt][i] = perm[i];

        /* compute parity: count inversions modulo 2 */
        int inv = 0;
        for (int i = 0; i < 7; ++i)
            for (int j = i + 1; j < 7; ++j)
                if (perm[i] > perm[j]) inv ^= 1;
        parity[*cnt] = inv; /* 0 even, 1 odd */

        (*cnt)++;
        return;
    }
    for (int v = 0; v < 7; ++v) {
        if (used & (1 << v)) continue;
        perm[depth] = v;
        gen_permutations(depth + 1, used | (1 << v), perm, perms, parity, cnt);
    }
}

/* Static storage for permutations */
static int g_perms[5040][7];
static char g_parity[5040];
static bool g_perms_ready = false;

/* Main sequence function */
seq_t a(idx_t n) {
    /* Prepare permutations once */
    if (!g_perms_ready) {
        size_t cnt = 0;
        int perm[7] = {0};
        gen_permutations(0, 0, perm, g_perms, g_parity, &cnt);
        g_perms_ready = true;
    }

    /* Initialize the constant matrix A (7x7) once */
    static seq_t A[7][7];
    static bool init_A = false;
    if (!init_A) {
        init_A = true;
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                A[i][j] = ((j) % (i + 1)) + 1;
            }
        }
    }

    /* Build the sum matrix S = A + A² + … + Aⁿ  (where n is the index) */
    uint64_t powers = (uint64_t)n + 1;  /* number of powers to include */
    seq_t P[7][7], S[7][7], tmp[7][7];
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j) {
            P[i][j] = A[i][j];
            S[i][j] = A[i][j];
        }

    for (uint64_t k = 2; k <= powers; ++k) {
        matrix_mul(P, A, tmp);        /* tmp = P * A   (next power) */
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 7; ++j) {
                P[i][j] = tmp[i][j];
                S[i][j] += tmp[i][j];
            }
    }

    /* Compute determinant of S using complete permutation expansion.
       The determinant is the sum over all permutations of sign * product
       of the selected entries.  The sign alternates parity of the
       permutation.  All products are positive, so we accumulate even
       contributions separately from odd ones and subtract at the end. */
    seq_t pos = 0;   /* sum of terms with even parity */
    seq_t neg = 0;   /* sum of terms with odd parity */

    for (size_t idx = 0; idx < 5040; ++idx) {
        seq_t prod = 1;
        for (int i = 0; i < 7; ++i)
            prod *= S[i][g_perms[idx][i]];
        if (g_parity[idx] == 0)
            pos += prod;
        else
            neg += prod;
    }

    /* Determinant should be positive; pos >= neg for this sequence. */
    return pos - neg;
}
