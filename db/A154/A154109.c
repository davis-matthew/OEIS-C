/*
 * A154109  Convolution triangle by rows, A004736 * (A154108 * 0^(n-k)); row
 *          sums = Bell numbers.
 * https://oeis.org/A154109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r - c + 1) * A154108[c] where r = row number of the
 *            linear index n (1‑based), c = column number within the row, and
 *            A154108 is defined by A[1] = 1, A[2] = 0, A[r] = B[r] - B[r‑1] -
 *            Σ_{k=1}^{r‑1} A[k] with B[r] the Bell numbers (B[0] = 0 for this
 *            recurrence)
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- Dependencies ---- */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ---- Typedefs – supplied by the build ---- */
typedef uint64_t idx_t;   /* index type – unsigned 64‑bit */

/* ---- Internal state ---- */
static seq_t *trueB      = NULL;   /* Bell numbers (B[0]=1) for recurrence */
static size_t trueB_len  = 0;
static size_t trueB_cap  = 0;

static seq_t *A          = NULL;   /* A154108 values, 1‑based */
static size_t A_len      = 0;
static size_t A_cap      = 0;
static seq_t cum_sum     = (seq_t)0;      /* Σ_{k=1}^{i-1} A[k] */

/* ---- Ensure enough space for an array ---- */
static void grow(void **ptr, size_t *cap, size_t need, size_t elem_size) {
    if (*cap >= need) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 1;
    while (new_cap < need) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    *ptr = tmp;
    *cap = new_cap;
}

/* ---- Compute Bell numbers B[0..up_to] (Bell[0] = 1) ---- */
static void ensure_trueB(size_t up_to) {
    if (trueB_len > up_to) return;
    grow((void **)&trueB, &trueB_cap, up_to + 1, sizeof(seq_t));
    if (trueB_len == 0) {
        trueB[0] = (seq_t)1;          /* B[0] = 1 */
        trueB_len = 1;
    }
    for (size_t i = trueB_len; i <= up_to; ++i) {
        /* B[i] = Σ_{k=0}^{i-1} C(i-1,k) * B[k] */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;
        size_t m = i - 1;
        for (size_t k = 0; k <= m; ++k) {
            sum += binom * trueB[k];
            if (k < m) {
                binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
            }
        }
        trueB[i] = sum;
    }
    trueB_len = up_to + 1;
}

/* ---- Compute A[1..up_to] using the recurrence ---- */
static void ensure_A(size_t up_to) {
    if (A_len >= up_to) return;
    /* Make sure Bell numbers up to up_to are ready */
    ensure_trueB(up_to);

    grow((void **)&A, &A_cap, up_to + 1, sizeof(seq_t));
    /* A[0] is unused; initialise if not set */
    if (A_len == 0) A[0] = (seq_t)0;

    for (size_t i = A_len + 1; i <= up_to; ++i) {
        seq_t B_i   = trueB[i];
        seq_t B_im1 = (i == 1 ? (seq_t)0 : trueB[i - 1]);  /* B[0] treated as 0 in recurrence */
        seq_t val   = B_i - B_im1 - cum_sum;
        A[i] = val;
        cum_sum += val;
    }
    A_len = up_to;
}

/* ---- Find row number r such that (r–1)*r/2 < n ≤ r*(r+1)/2 ---- */
static idx_t row_of(idx_t n) {
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* ---- Public function implementing A154109 ---- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    idx_t r = row_of(n);
    idx_t offset = (r - 1) * r / 2;
    idx_t c = n - offset;          /* 1‑based column within the row */

    ensure_A((size_t)r);           /* compute A[1..r] as needed */

    seq_t mult = (seq_t)(r - c + 1);
    seq_t result = mult * A[(size_t)c];
    return result;
}
