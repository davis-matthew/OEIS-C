/*
 * A125351  Number of base 14 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Number of base 14 circular n‑digit numbers with adjacent digits
 *            differing by 4 or less. a(n) = trace(A^n) for n≥1, with a(0)=1.
 *            A is the 14×14 adjacency matrix with A[i][j] = 1 if |i-j| ≤ 4,
 *            else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

/* Helper to safely multiply two seq_t values.
   For integral types up to 64 bits use 128‑bit arithmetic if available.
   For arbitrary‑precision types the built‑in operator* is used. */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
#if defined(__cplusplus)
    if constexpr (std::numeric_limits<seq_t>::is_integer) {
        if constexpr (sizeof(seq_t) <= 8) {
            return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
        } else {
            return a * b;  /* 128‑bit built‑in or user defined */
        }
    } else {
        return a * b;
    }
#else
#if defined(__SIZEOF_INT128__) && !defined(__MINGW32__)
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
#else
    return a * b;
#endif
#endif
}

/* Size of the adjacency graph (digits 0‑13). */
#define B 14
#define SZ (B * B)

/* Generate the 14×14 adjacency matrix (entries 0/1). */
static void init_adj(seq_t *adj)
{
    static bool done = false;
    if (done) return;
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            int diff = i - j;
            if (diff < 0) diff = -diff;
            adj[i * B + j] = (diff <= 4) ? (seq_t)1 : (seq_t)0;
        }
    }
    done = true;
}

/* Matrix multiplication: dest = a * b (both B×B). */
static void mat_mult(seq_t *dest, const seq_t *a, const seq_t *b)
{
    seq_t tmp[SZ];
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < B; ++k) {
                sum += mul_seq(a[i * B + k], b[k * B + j]);
            }
            tmp[i * B + j] = sum;
        }
    }
    for (int i = 0; i < SZ; ++i) dest[i] = tmp[i];
}

/* Compute A^n (modulo the arithmetic of seq_t). */
static void mat_pow(seq_t *res, const seq_t *base, idx_t exp)
{
    seq_t identity[SZ];
    for (int i = 0; i < SZ; ++i) identity[i] = (seq_t)0;
    for (int i = 0; i < B; ++i) identity[i * B + i] = (seq_t)1;

    seq_t power[SZ];
    for (int i = 0; i < SZ; ++i) power[i] = base[i];

    for (int i = 0; i < SZ; ++i) res[i] = identity[i];

    while (exp > 0) {
        if (exp & 1ULL) {
            mat_mult(res, res, power);
        }
        mat_mult(power, power, power);
        exp >>= 1;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    static seq_t adjacency[SZ];
    init_adj(adjacency);

    seq_t power[SZ];
    mat_pow(power, adjacency, n);

    seq_t trace = (seq_t)0;
    for (int i = 0; i < B; ++i) {
        trace += power[i * B + i];
    }
    return trace;
}
