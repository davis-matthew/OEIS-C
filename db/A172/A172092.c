/*
 * A172092  Triangle, read by rows, T(n,k,q) = c(k,q) + c(n-k,q) - c(n, q)
 *          where c(n,q) = Product_{j=1..n-1} ((q^(j+1) - 1)/(q-1)) and q = 3.
 * https://oeis.org/A172092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   The triangle read by rows is T(n,k) = c(k)+c(n-k)-c(n) with
 *            c(0)=c(1)=1 and for n≥2 c(n) = ∏_{m=2}^{n} (3^m-1)/2. The linear
 *            sequence a(n) is obtained by scanning the triangle row by row.
 *            That is, for the unique r with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 let
 *            k = n - r*(r+1)/2 ; then a(n) = T(r,k)
 * Range:     i8 n <= 10, i16 n <= 15, i32 n <= 28, i64 n <= 55, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *c_cache = NULL;          /* memoised values of c(n)        */
static idx_t  c_cache_len  = 0;        /* number of computed elements    */
static idx_t  c_cache_cap  = 0;        /* allocated capacity             */
static seq_t  pow3 = 0;                /* current 3^current_index        */

/* Ensure that c_cache contains values up to 'upto' (inclusive). */
static void
ensure_c_cache(idx_t upto)
{
    if (upto < c_cache_len)
        return;                       /* already available            */

    if (c_cache_len == 0) {            /* initialisation               */
        c_cache_cap = 2;
        c_cache = (seq_t *)malloc(sizeof(seq_t) * c_cache_cap);
        if (!c_cache)
            return;                    /* allocation failure            */
        c_cache[0] = (seq_t)1;         /* c(0) = 1                      */
        c_cache[1] = (seq_t)1;         /* c(1) = 1                      */
        c_cache_len = 2;
        pow3 = (seq_t)9;               /* 3^2 is ready for next index */
    }

    /* Grow the cache if needed. */
    while (c_cache_len <= upto) {
        if (c_cache_len >= c_cache_cap) {
            size_t new_cap = c_cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(c_cache, sizeof(seq_t) * new_cap);
            c_cache = tmp;
            c_cache_cap = new_cap;
        }

        idx_t i = c_cache_len;          /* next index to compute (i ≥ 2)   */
        seq_t term = (pow3 - (seq_t)1) / (seq_t)2;  /* (3^i-1)/2 */
        seq_t val  = c_cache[i - 1] * term;
        c_cache[i] = val;
        c_cache_len++;

        pow3 = pow3 * (seq_t)3;          /* advance to 3^(i+1)              */
    }
}

/* Returns the a(n) value of OEIS A172092. */
seq_t
a(idx_t n)
{
    /* Locate the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        seq_t off = (seq_t)mid * ((seq_t)mid + (seq_t)1) / (seq_t)2;
        if (off <= (seq_t)n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    seq_t row_start = (seq_t)r * ((seq_t)r + (seq_t)1) / (seq_t)2;
    idx_t k = (idx_t)((seq_t)n - row_start);

    /* Ensure required c values are available. */
    ensure_c_cache(r);
    ensure_c_cache(k);
    ensure_c_cache(r - k);

    seq_t c_r  = c_cache[r];
    seq_t c_k  = c_cache[k];
    seq_t c_ik = c_cache[r - k];

    return -c_r + c_k + c_ik;          /* T(r,k) = c(k)+c(r-k)-c(r) */
}
