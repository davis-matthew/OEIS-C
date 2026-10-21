/*
 * A059024  Triangle of Stirling numbers of order 5.
 * https://oeis.org/A059024
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, tabf, nice
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 41, u64 n <= 95, u128 n <= 236,
 *            big any n
 * Helpers:   binomial, index_to_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 5‑associated Stirling numbers of the second kind (OEIS A059024)
 *
 * Recurrence (r = 5):
 *     S5(n+1,k) = k * S5(n,k) + binomial(n,4) * S5(n-4,k-1)
 *     with S5(0,0) = 1 and S5(m,0) = 0 for m > 0.
 *
 * ψ(n) :
 *   a(n) is obtained by writing the triangle row‑wise
 *   (rows n = 5,6,7,…) and taking the (n‑5)-th element of that
 *   flattened list.  For example:
 *      n = 5  → S5(5,1)
 *      n =10  → S5(10,1)
 *      n =11  → S5(10,2)
 *
 * The code keeps a static memoised 2‑D array of the triangle.
 * It works for all builds: for integral seq_t it uses C memory
 * functions; for arbitrary‑precision seq_t it uses C++ new[].
 */

#include <stddef.h>
#include "../utils/binomial.h"          /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/index_to_position.h"

static seq_t **S = NULL;     /* S[n][k]  (k may be 0) */
static idx_t  computed_max = 0;   /* largest n already computed */

/* allocation for arbitrary‑precision types (C++ new) or integral types (calloc) */
static seq_t* allocate_seq(idx_t size)
{
#ifdef __cplusplus
    return new seq_t[size];
#else
    return (seq_t*)calloc((size_t)size, sizeof(seq_t));
#endif
}

static seq_t** allocate_ptrs(idx_t size)
{
#ifdef __cplusplus
    return new seq_t*[size];
#else
    return (seq_t**)calloc((size_t)size, sizeof(seq_t*));
#endif
}

static void init_base(void)
{
    if (S == NULL) {
        S = allocate_ptrs(1);       /* row 0 only */
        S[0] = allocate_seq(1);
        S[0][0] = 1;                /* S5(0,0) = 1 */
        computed_max = 0;
    }
}

static void ensure_computed(idx_t n)
{
    if (n <= computed_max) return;

    init_base();

    /* grow pointer array to size n+1 */
    if (S != NULL) {
#ifdef __cplusplus
        seq_t** tmp = new seq_t*[n + 1];
        for (idx_t i = 0; i <= computed_max; ++i) tmp[i] = S[i];
        delete[] S;
        S = tmp;
#else
        S = realloc(S, (size_t)(n + 1) * sizeof(seq_t*));
#endif
    }

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        idx_t kmax = i / 5;              /* maximum k with non‑zero value */
        S[i] = allocate_seq(kmax + 1);   /* indices 0 … kmax */

        /* S5(i,0) = 0 for i > 0; already zeroed by calloc/new[] */

        if (i >= 5) {
            seq_t bin = binomial(i - 1, 4);          /* (i‑1 choose 4) */
            for (idx_t k = 1; k <= kmax; ++k) {
                seq_t val = 0;
                if (k <= (i - 1) / 5) val += (seq_t)k * S[i - 1][k];
                if (k - 1 <= (i - 5) / 5) val += bin * S[i - 5][k - 1];
                S[i][k] = val;
            }
        }
    }

    computed_max = n;
}

/* exported sequence function */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;  /* terms below the offset are undefined */

    idx_t m, k;
    index_to_position(n, &m, &k);

    ensure_computed(m);

    return S[m][k];
}
