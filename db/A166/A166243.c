/*
 * A166243  Villegas-Zagier polynomials (listing coefficients from lowest to
 *          highest degree).
 * https://oeis.org/A166243
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   * V(-1) = 0, * V(0) = 1, * for n >= 0: * V(n+1) =
 *            (8*x^3-1)*V'(n) - (16*n+3)*x^2*V(n) - 4*n*(2*n-1)*x*V(n-1) * *
 *            The sequence a(n) (offset -1) is obtained by concatenating the
 *            coefficients * of V(‑1), V(0), V(1), … in increasing degree
 *            order
 * Range:     i8 n <= 25, i16 n <= 53, i32 n <= 89, i64 n <= 214,
 *            i128 n <= 650, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


/* ---------- internal helpers ---------- */

static inline idx_t isqrt_idx(idx_t x) {
    unsigned __int128 r = 0;
    unsigned __int128 bit = ((unsigned __int128)1) << 62;
    while (bit > 0) {
        unsigned __int128 tmp = r + bit;
        if ((unsigned __int128)x >= tmp * tmp)
            r = tmp;
        bit >>= 1;
    }
    return (idx_t)r;
}

static seq_t **V = NULL;          /* polynomial coefficient arrays  */
static size_t *Vlen = NULL;       /* length of each array           */
static int capacity = 0;          /* allocated array capacity      */
static int max_k = -1;            /* largest k computed (-1 for none) */

/* initialise V(-1) and V(0) */
static void init_polys(void) {
    if (V != NULL)
        return;
    capacity = 4;
    V = (seq_t **)malloc(capacity * sizeof(seq_t *));
    Vlen = (size_t *)malloc(capacity * sizeof(size_t));

    /* V(-1) */
    V[0] = (seq_t *)malloc(sizeof(seq_t));
    V[0][0] = (seq_t)0;
    Vlen[0] = 1;

    /* V(0) */
    V[1] = (seq_t *)malloc(sizeof(seq_t));
    V[1][0] = (seq_t)1;
    Vlen[1] = 1;

    max_k = 0;
}

/* ensure polynomials up to k are computed (k >= 0) */
static void ensure_k(idx_t k_needed) {
    init_polys();
    while (max_k < k_needed) {
        int new_k = max_k + 1;                /* compute V(new_k) */
        int idx_new = new_k + 1;              /* array index of V(new_k) */

        /* grow storage if necessary */
        if (idx_new >= capacity) {
            capacity *= 2;
            V = (seq_t **)realloc(V, capacity * sizeof(seq_t *));
            Vlen = (size_t *)realloc(Vlen, capacity * sizeof(size_t));
        }

        /* reference existing polynomials */
        seq_t *prev1 = V[new_k];          /* V(new_k-1)   */
        seq_t *prev2 = V[new_k - 1];      /* V(new_k-2)   */
        size_t len1 = Vlen[new_k];
        size_t len2 = Vlen[new_k - 1];
        int n_rec = new_k - 1;            /* n in the recurrence */

        /* compute derivative of prev1 (V(n)) */
        size_t lenD = (len1 > 0 ? len1 - 1 : 0);
        seq_t *D = NULL;
        if (lenD > 0) {
            D = (seq_t *)malloc(lenD * sizeof(seq_t));
            for (size_t i = 1; i < len1; ++i)
                D[i - 1] = prev1[i] * (seq_t)i;
        }

        /* result polynomial */
        size_t target_len = 2 * (size_t)new_k + 1;
        seq_t *res = (seq_t *)calloc(target_len, sizeof(seq_t));

        /* (8*x^3-1)*V'(n) */
        for (size_t i = 0; i < lenD; ++i) {
            res[i] = res[i] - D[i];                  /* -D[i]   */
            res[i + 3] = res[i + 3] + (seq_t)8 * D[i];/* +8*D[i] */
        }
        if (D) free(D);

        /* -(16*n+3)*x^2*V(n) */
        seq_t factor2 = (seq_t)(16 * n_rec + 3);
        for (size_t i = 0; i < len1; ++i)
            res[i + 2] = res[i + 2] - factor2 * prev1[i];

        /* -4*n*(2*n-1)*x*V(n-1) */
        seq_t factor3 = (seq_t)(4 * n_rec * (2 * n_rec - 1));
        for (size_t i = 0; i < len2; ++i)
            res[i + 1] = res[i + 1] - factor3 * prev2[i];

        /* store computed polynomial */
        V[idx_new] = res;
        Vlen[idx_new] = target_len;
        max_k = new_k;
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n) {
    if (n == (idx_t)-1)
        return (seq_t)0;                     /* V(-1) */

    idx_t pos = n + 1;                      /* zero‑based position */
    idx_t k = isqrt_idx(pos - 1);           /* k with k^2+1 <= pos <= (k+1)^2 */

    ensure_k(k);

    idx_t start = k * k + 1;                /* first position of V(k) */
    idx_t offset = pos - start;             /* index within V(k) */

    int poly_idx = (int)(k + 1);            /* array index of V(k) */
    return V[poly_idx][(size_t)offset];
}
