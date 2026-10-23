/*
 * A125480  Number of base 23 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑23 circular n‑digit numbers whose adjacent
 *            digits differ by ≤ 9. Computed as the trace of Aⁿ where A is the
 *            23×23 adjacency matrix with A[i][j] = 1 if |i–j|≤9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The graph has 23 vertices, so we hard‑code the size as a compile‑time
   constant to avoid VLA errors. */
enum { B = 23 };

/* Helper: multiply two seq_t values safely.  In C++ arithmetic, the
   overloaded operators give the exact product.  In C, we use a 128‑bit
   intermediate when available to avoid overflow of 64‑bit products. */
#if defined(__cplusplus)
static inline seq_t mul_seq(seq_t x, seq_t y) { return x * y; }
#elif defined(__SIZEOF_INT128__)
static inline seq_t mul_seq(seq_t x, seq_t y) { return (seq_t)(((__uint128_t)x) * y); }
#else
static inline seq_t mul_seq(seq_t x, seq_t y) { return x * y; }
#endif

/* Multiply two B×B matrices of seq_t, result in out.  The multiplication
   is performed in the usual O(B³) manner. */
static void mul_mat(const seq_t a[B][B], const seq_t b[B][B], seq_t out[B][B])
{
    for (unsigned i = 0; i < B; ++i) {
        for (unsigned j = 0; j < B; ++j) {
            seq_t sum = 0;
            for (unsigned k = 0; k < B; ++k) {
                sum = sum + mul_seq(a[i][k], b[k][j]);
            }
            out[i][j] = sum;
        }
    }
}

/* Copy matrix src into dest. */
static void copy_mat(const seq_t src[B][B], seq_t dest[B][B])
{
    for (unsigned i = 0; i < B; ++i)
        for (unsigned j = 0; j < B; ++j)
            dest[i][j] = src[i][j];
}

/* The adjacency matrix for base‑23 with |i–j| ≤ 9.  It is built once. */
static seq_t adjacency[B][B];
static int adjacency_inited = 0;
static void init_adjacency(void)
{
    if (adjacency_inited) return;
    for (unsigned i = 0; i < B; ++i) {
        for (unsigned j = 0; j < B; ++j) {
            unsigned diff = (i > j) ? i - j : j - i;
            adjacency[i][j] = (diff <= 9) ? 1 : 0;
        }
    }
    adjacency_inited = 1;
}

/* Sequence generator. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    init_adjacency();

    /* result = identity matrix */
    seq_t result[B][B];
    seq_t base[B][B];
    seq_t tmp[B][B];
    for (unsigned i = 0; i < B; ++i) {
        for (unsigned j = 0; j < B; ++j) {
            result[i][j] = (i == j) ? 1 : 0;
            base[i][j]   = adjacency[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mul_mat(result, base, tmp);
            copy_mat(tmp, result);
        }
        mul_mat(base, base, tmp);
        copy_mat(tmp, base);
        exp >>= 1;
    }

    /* Trace of result is the number of closed walks of length n. */
    seq_t trace = 0;
    for (unsigned i = 0; i < B; ++i) trace = trace + result[i][i];
    return trace;
}
