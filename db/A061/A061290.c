/*
 * A061290  Square array read by antidiagonals of T(n,k) = T(n-1,k) + T(n-1,
 *          floor(k/2)) with T(0,0)=1.
 * https://oeis.org/A061290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k)=T(n-1,k)+T(n-1,floor(k/2)),
 *            T(0,0)=1. Using the closed form T(n,k) = 2^n –
 *            Σ_{i=0}^{⌊log₂(k)⌋} C(n,i) (k>0), T(n,0)=2^n
 * Range:     u8 n <= 43, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility helpers: integer square root and floor_log2 for 64‑bit values. */
static inline idx_t floor_sqrt(idx_t x)
{
    /* Largest power of 4 ≤ x */
    idx_t bit = (idx_t)1 << 62;           /* works for 64‑bit */
    while (bit > x) bit >>= 2;
    idx_t res = 0;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

static inline unsigned floor_log2_uint64(idx_t v)
{
    /* v > 0 when called */
    return 63 - __builtin_clzll(v);
}

/* Returns 2^exp as seq_t.  Uses successive multiplication to keep the
   implementation independent from any specific seq_t type. */
static inline seq_t pow2_uint(idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) r *= (seq_t)2;
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* --- determine (row, col) from the antidiagonal index n --- */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;  /* 8*n+1 */
    idx_t s = floor_sqrt((idx_t)t);                    /* floor sqrt(8*n+1) */
    idx_t d = (s - 1) / 2;                             /* diagonal number */
    unsigned __int128 prev = (unsigned __int128)d * (d + 1) / 2; /* cum_{d-1} */
    idx_t row = n - (idx_t)prev;
    idx_t col = d - row;                               /* row + col = d */

    /* --- compute T(row, col) --- */
    if (col == 0) {
        /* T(row,0) = 2^row */
        return pow2_uint(row);
    }

    /* Sum of binomial coefficients up to floor_log2(col) */
    unsigned m = floor_log2_uint64(col);
    seq_t sum = (seq_t)0;
    seq_t comb = (seq_t)1;                 /* C(row,0) */
    for (idx_t i = 0; i <= m; ++i) {
        if (i > 0) {
            comb = comb * (seq_t)(row - i + 1) / (seq_t)i;  /* C(row,i) */
        }
        sum += comb;
    }

    seq_t total = pow2_uint(row);          /* 2^row */
    return total - sum;                    /* desired T(row, col) */
}
