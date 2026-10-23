/*
 * A187152  Triangle T(m,n), read by rows: Number of bipartite labeled graphs
 *          (V,E) with vertices A={a_1,...,a_m} and B={b_1,...,b_n} where for
 *          any vertex in V at most one edge in E is allowed. Additionally, an
 *          edge {a_k,b_l} is allowed only when |k-l|<=1.
 * https://oeis.org/A187152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(m,n) by rows. For a
 *            given n, find the smallest m with m(m+1)/2 ≥ n. Let k = n –
 *            m(m-1)/2 (1 ≤ k ≤ m). If k == m then a(n) = D(m) (diagonal
 *            entry) Else a(n) = U(k) (off‑diagonal entry) D(m) is the
 *            diagonal of the triangle: D(1)=2, D(2)=7, D(3)=22 D(j)=3*D...
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"

static seq_t *D_array = NULL;        /* Diagonal values, 1‑based */
static seq_t *U_array = NULL;        /* Off‑diagonal values, 1‑based */
static size_t D_len = 0;             /* Highest index computed +1 */
static size_t U_len = 0;

/* Ensure that D_array contains values up to index n (1‑based). */
static void ensure_D(size_t n)
{
    if (n + 1 <= D_len) return;        /* already computed */

    size_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(D_array, new_len * sizeof(seq_t));
    if (!tmp) exit(1);                  /* abort on allocation failure */
    D_array = tmp;
    D_len = new_len;

    for (size_t i = 1; i <= n; ++i) {
        if (i == 1) D_array[i] = (seq_t)2;
        else if (i == 2) D_array[i] = (seq_t)7;
        else if (i == 3) D_array[i] = (seq_t)22;
        else
            D_array[i] = 3 * D_array[i-1] + D_array[i-2] - D_array[i-3];
    }
}

/* Ensure that U_array contains values up to index n (1‑based). */
static void ensure_U(size_t n)
{
    if (n + 1 <= U_len) return;        /* already computed */

    size_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(U_array, new_len * sizeof(seq_t));
    if (!tmp) exit(1);                  /* abort on allocation failure */
    U_array = tmp;
    U_len = new_len;

    for (size_t i = 1; i <= n; ++i) {
        if (i == 1) U_array[i] = (seq_t)3;
        else if (i == 2) U_array[i] = (seq_t)10;
        else if (i == 3) U_array[i] = (seq_t)32;
        else
            U_array[i] = 3 * U_array[i-1] + U_array[i-2] - U_array[i-3];
    }
}

/* Return diagonal value D(m). */
static inline seq_t get_D(idx_t m)
{
    ensure_D((size_t)m);
    return D_array[(size_t)m];
}

/* Return off‑diagonal value U(k). */
static inline seq_t get_U(idx_t k)
{
    ensure_U((size_t)k);
    return U_array[(size_t)k];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find m such that m(m+1)/2 ≥ n, using binary search. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    idx_t prev = (unsigned __int128)(m - 1) * m / 2;
    idx_t k = n - prev;    /* 1‑based column index */

    if (k == m)
        return get_D(m);
    else
        return get_U(k);
}
