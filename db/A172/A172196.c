/*
 * A172196  Triangle T(n, k, q) = 1 + abs(c(n,q) - c(k,q))*abs(c(n,q) - c(n-k,
 *          q)), where c(n,q) = Product_{j=1..n} (1 - q^j) and q = 2, read by
 *          rows.
 * https://oeis.org/A172196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = T(n, k, 2) where * T(n, k, q) = 1 + |c(n,q) – c(k,q)| *
 *            |c(n,q) – c(n-k,q)| * with c(n,q) = Π_{j=1..n} (1 – q^j) and q =
 *            2 * The sequence is read by rows of the triangle; a(0) is the
 *            first * triangle entry. The values c(n) are stored by magnitude
 *            and sign: sign * alternates with n, and mag...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 36, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of the external linkage function required by the
 * build system.  The exact typedefs for seq_t and idx_t are supplied at
 * link time. */
seq_t a(idx_t n);

/* ------------------------------------------------------------------
 * Internal state: cached values of |c(n)| and signs of c(n).
 * c_val[n]  holds the magnitude Π_{j=1..n} (2^j – 1).
 * c_sign[n] is +1 for n even, –1 for n odd.
 * ------------------------------------------------------------------ */
static seq_t *c_val = NULL;     /* magnitude of c(n)            */
static int   *c_sign = NULL;    /* ±1                           */
static idx_t   c_len = 0;       /* number of cached entries     */
static seq_t  pow_of_two = 0;   /* current value of 2^c_len      */

/* ------------------------------------------------------------------
 * Ensure that c(n) is available for all n <= max_n.
 * The routine expands the arrays lazily and keeps pow_of_two updated.
 * ------------------------------------------------------------------ */
static void ensure_c(idx_t max_n)
{
    if (max_n < c_len) return;            /* already satisfied */

    /* Allocate (or enlarge) the arrays. */
    if (c_len == 0) {
        c_val   = (seq_t*)malloc(sizeof(seq_t));
        c_sign  = (int  *)malloc(sizeof(int));
        if (!c_val || !c_sign) abort();
        /* Initialize c(0) */
        c_val[0]  = (seq_t)1;
        c_sign[0] = 1;
        pow_of_two = (seq_t)1;            /* 2^0 */
        c_len = 1;
    } else {
        c_val  = (seq_t*)realloc(c_val, (max_n + 1) * sizeof(seq_t));
        c_sign = (int  *)realloc(c_sign, (max_n + 1) * sizeof(int));
        if (!c_val || !c_sign) abort();
    }

    /* Compute new terms up to max_n. */
    for (idx_t i = c_len; i <= max_n; ++i) {
        /* Update pow_of_two to 2^i. */
        pow_of_two = pow_of_two * (seq_t)2;
        /* Factor (2^i – 1). */
        seq_t factor = pow_of_two - (seq_t)1;
        /* c(i) = c(i-1) * factor, sign alternates. */
        seq_t prod = c_val[i-1] * factor;
        c_val[i]  = prod;
        c_sign[i] = -c_sign[i-1];
    }
    c_len = max_n + 1;
}

/* ------------------------------------------------------------------
 * Compute |c(a) – c(b)| when the values are stored as magnitude and sign.
 * ------------------------------------------------------------------ */
static seq_t abs_diff(idx_t a, idx_t b)
{
    if (c_sign[a] == c_sign[b]) {
        /* Same sign: subtract magnitudes. */
        if (c_val[a] >= c_val[b])
            return c_val[a] - c_val[b];
        else
            return c_val[b] - c_val[a];
    } else {
        /* Opposite signs: add magnitudes. */
        return c_val[a] + c_val[b];
    }
}

/* ------------------------------------------------------------------
 * Main function computing the sequence entry at zero‑based index n.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    /* Use 128‑bit to avoid overflow on large n. */
    while (((unsigned __int128)(r + 1) * (r + 2)) <= (unsigned __int128)2 * n) {
        ++r;
    }

    /* Position within the row. */
    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    /* Ensure c values up to max(r, k, r-k) are computed. */
    idx_t max_needed = r;
    if (k > max_needed)          max_needed = k;
    if (r > k && (r - k) > max_needed)  max_needed = r - k;
    ensure_c(max_needed);

    /* Triangle entry: 1 + |c(r) – c(k)| * |c(r) – c(r-k)| */
    seq_t diff1 = abs_diff(r, k);
    seq_t diff2 = abs_diff(r, r - k);
    seq_t prod  = diff1 * diff2;
    return prod + (seq_t)1;
}
