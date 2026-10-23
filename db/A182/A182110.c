/*
 * A182110  Irregular triangle read by rows: generating function counting
 *          rotationally distinct n X n tatami tilings with n monomers and
 *          exactly k vertical dimers.
 * https://oeis.org/A182110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term obtained by reading the triangular * array
 *            T(n,k) row‑by‑row. For each n≥2 let * * T_n(z) =
 *            2*∑_{i=1}^{⌊(n‑1)/2⌋} S_{n‑i‑2}(z)·S_{i‑1}(z)·z^{n‑i‑1} * +
 *            (S_{⌊(n‑2)/2⌋}(z))^2 * * where S_k(z)=∏_{j=1}^{k}(1+z^j). The
 *            coefficients of T_n(z) for * k=0,1,…,binom(n‑1,2) are
 *            concaten...
 * Range:     u8 n <= 190, u16 n <= 1205, u32 n <= 7353, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----------  Helpers for S_k(z)  ---------- */
static seq_t **S_coeff = NULL;   /* pointers to coefficient arrays of S_k(z) */
static size_t *S_deg   = NULL;   /* maximum degree of S_k(z) (size-1) */
static size_t S_count  = 0;       /* number of S_k(z) already computed   */
static size_t S_cap    = 0;       /* allocated capacity for the arrays   */

/* Ensure that S_k(z) for all k ≤ max_k are available. */
static void ensure_S(size_t max_k)
{
    if (max_k < S_count) return;

    /* grow capacity if necessary */
    if (max_k + 1 > S_cap) {
        size_t new_cap = (S_cap == 0) ? 16 : S_cap * 2;
        while (new_cap <= max_k) new_cap <<= 1;
        S_coeff = (seq_t**)realloc(S_coeff, new_cap * sizeof(seq_t*));
        S_deg   = (size_t*)realloc(S_deg,   new_cap * sizeof(size_t));
        S_cap   = new_cap;
    }

    /* compute missing S_k(z) */
    for (size_t k = S_count; k <= max_k; ++k) {
        if (k == 0) {
            /* S_0(z) = 1 */
            S_coeff[0] = (seq_t*)malloc(sizeof(seq_t));
            S_coeff[0][0] = (seq_t)1;
            S_deg[0] = 0;
        } else {
            /* S_k(z) = S_{k-1}(z) * (1 + z^k) */
            size_t prev_deg = S_deg[k-1];
            size_t new_deg  = prev_deg + k;             /* degree increase by k */
            seq_t *new_poly = (seq_t*)calloc(new_deg + 1, sizeof(seq_t));
            for (size_t i = 0; i <= prev_deg; ++i) {
                new_poly[i]     += S_coeff[k-1][i];          /* coefficient *1 */
                new_poly[i + k] += S_coeff[k-1][i];          /* coefficient *z^k */
            }
            S_coeff[k] = new_poly;
            S_deg[k]   = new_deg;
        }
    }
    S_count = max_k + 1;
}

/* ----------  Storage of the flattened sequence  ---------- */
static seq_t *seq_global = NULL;
static size_t seq_len   = 0;   /* number of terms stored so far   */
static size_t seq_cap   = 0;   /* allocated capacity             */
static size_t next_row  = 2;   /* next n for which we compute T_n */

/* Append a block of coeffs to the global array (grow as needed). */
static void append_to_seq(const seq_t *coeff, size_t len)
{
    if (seq_len + len > seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 2048 : seq_cap * 2;
        while (seq_len + len > new_cap) new_cap <<= 1;
        seq_global = (seq_t*)realloc(seq_global, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
    for (size_t i = 0; i < len; ++i) seq_global[seq_len++] = coeff[i];
}

/* Compute T_n(z) coefficients and append them to seq_global. */
static void compute_and_append_T(size_t n)
{
    /* The polynomial has degree binom(n‑1,2) */
    size_t deg_T = (n - 1) * (n - 2) / 2;
    seq_t *T = (seq_t*)calloc(deg_T + 1, sizeof(seq_t));

    /* Ensure necessary S_k(z) are ready.  For the sums we need
     * S_k with k up to n‑3 (cause i=1 gives k=n‑3).  (Even for n=2 the
     * loop is empty.)
     */
    if (n >= 3) ensure_S(n - 3);
    else        ensure_S(0);

    /* First summation part */
    size_t half = (n - 1) / 2;
    for (size_t i = 1; i <= half; ++i) {
        size_t idx1 = n - i - 2;   /* S_{n-i-2} */
        size_t idx2 = i - 1;       /* S_{i-1}   */
        size_t shift = n - i - 1;  /* z^{n-i-1} */

        const seq_t *S1 = S_coeff[idx1];
        const seq_t *S2 = S_coeff[idx2];
        size_t deg1 = S_deg[idx1];
        size_t deg2 = S_deg[idx2];

        for (size_t a = 0; a <= deg1; ++a) {
            for (size_t b = 0; b <= deg2; ++b) {
                size_t pos = a + b + shift;
                if (pos > deg_T) continue;
                seq_t tmp = S1[a] * S2[b];
                T[pos] += tmp + tmp;   /* multiply by 2 */
            }
        }
    }

    /* Square part */
    size_t c = (n - 2) / 2;            /* floor((n‑2)/2) */
    const seq_t *Sc = S_coeff[c];
    size_t degC = S_deg[c];
    for (size_t p = 0; p <= degC; ++p) {
        for (size_t q = 0; q <= degC; ++q) {
            size_t pos = p + q;
            if (pos > deg_T) continue;
            T[pos] += Sc[p] * Sc[q];
        }
    }

    /* Append the coefficients of T_n to the global sequence */
    append_to_seq(T, deg_T + 1);
    free(T);
}

/* ----------  Public function required by the framework  ---------- */
seq_t a(idx_t n)
{
    /* Generate rows until we have a term for the requested index */
    while (seq_len <= (size_t)n) {
        compute_and_append_T(next_row);
        ++next_row;
    }
    return seq_global[(size_t)n];
}
