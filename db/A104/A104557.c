/*
 * A104557  Triangle T, read by rows, such that the unsigned columns of the
 *          matrix inverse when read downwards equals the rows of T read
 *          backwards, with T(n,n)=1 and T(n,n-1) =
 *          floor((n+1)/2)*floor((n+2)/2).
 * https://oeis.org/A104557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(floor((sqrt(8n+1)-1)/2), n -
 *            floor((sqrt(8n+1)-1)/2)*(floor((sqrt(8n+1)-1)/2)+1)/2) where
 *            T(n,k) = (n-k)! * C(n - ⌊k/2⌋, ⌈k/2⌉) * C(n - ⌈k/2⌉, ⌊k/2⌋)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom, int_sqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/int_sqrt_idx.h"

static seq_t *fact = NULL;          /* factorial array */
static idx_t  fact_size = 0;        /* number of factorials computed (fact[i] for 0<=i<fact_size) */
static idx_t  fact_cap  = 0;        /* current capacity of the array */

/* Ensure factorials up to m are computed and stored. */
static void ensure_factorials(idx_t m)
{
    if (fact == NULL) {
        fact_cap = 16;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        fact[0] = 1;      /* 0! = 1 */
        fact_size = 1;
    }
    if (m >= fact_cap) {
        size_t new_cap = fact_cap;
        while (new_cap <= m) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact = tmp;
        fact_cap = (idx_t)new_cap;
    }
    while (fact_size <= m) {
        idx_t i = fact_size;
        fact[i] = fact[i-1] * (seq_t)i;
        fact_size = i + 1;
    }
}

/* Computes T(n,k) using the formula from the OEIS entry. */
static seq_t compute_T(idx_t n, idx_t k)
{
    /* n >= k is assumed by the caller. */
    idx_t a = k / 2;          /* floor(k/2) */
    idx_t b = (k + 1) / 2;    /* ceil(k/2) */

    idx_t n_fact = n - k;
    ensure_factorials(n_fact);
    seq_t fact_term = fact[n_fact];

    seq_t bin1 = binom(n - a, b);
    seq_t bin2 = binom(n - b, a);

    return fact_term * bin1 * bin2;
}

/* Public function computing A104557. */
seq_t a(idx_t n)
{
    /* Locate the row r and column c of the triangle. */
    idx_t t = 8 * n + 1;
    idx_t sqrt_t = int_sqrt_idx(t);
    idx_t r = (sqrt_t - 1) / 2;
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;

    return compute_T(r, c);
}
