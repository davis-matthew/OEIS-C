/*
 * A015689  Inverse of 1680th cyclotomic polynomial.
 * https://oeis.org/A015689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) is the coefficient of xⁿ in the reciprocal power series 1 /
 *            Φ₁₆₈₀(x) (the 1680‑th cyclotomic polynomial). Let Φ₁₆₈₀(x) =
 *            ∑_{k=0}^{φ(1680)} c_k xᵏ (c₀ = 1). Then a(0) = 1 and for n ≥ 1
 *            a(n) = –∑_{k=1}^{φ(1680)} c_k · a(n–k). (φ(1680) = 384)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Macros for allocating and freeing arrays that contain seq_t elements.
   In C++ the type may have a non‑trivial default constructor, so
   value‑initialization (new T[...]() ) is used.  In C the array is
   allocated with calloc to obtain zeroed memory. */
#ifdef __cplusplus
#define ALLOC_SEQ_ARRAY(n)   new seq_t[(n)]()
#define FREE_SEQ_ARRAY(p)    delete[] (p)
#else
#define ALLOC_SEQ_ARRAY(n)   ((seq_t*)calloc((n), sizeof(seq_t)))
#define FREE_SEQ_ARRAY(p)    free(p)
#endif

/* Divisors of 1680 with μ(d) = +1 and –1 for the factorisation
   Φ₁₆₈₀(x) = ∏_{d|1680} (x^{1680/d} – 1)^{μ(d)} . */
static const idx_t pos_k[8] = {
    1680ULL, 280ULL, 168ULL, 120ULL, 112ULL, 80ULL, 48ULL, 8ULL
};
static const idx_t neg_k[8] = {
    840ULL, 560ULL, 336ULL, 240ULL, 56ULL, 40ULL, 24ULL, 16ULL
};

/* Coefficients of Φ₁₆₈₀(x).  φ(1680) = 384. */
static seq_t coef[385];
static idx_t poly_deg = 0;

/* Cache for computed a(n) values. */
static seq_t *cache = NULL;
static size_t cached_len = 0;  /* number of values stored (≥0) */
static size_t cached_cap = 0;  /* allocated capacity */

/* Multiply polynomial P by (x^k - 1) in place.  P points to the array and
   deg is the current degree.  After the call deg is increased by k. */
static void mul_by_factor(seq_t **P_ptr, size_t *deg_ptr, idx_t k)
{
    seq_t *P = *P_ptr;
    size_t old_deg = *deg_ptr;
    size_t new_deg = old_deg + (size_t)k;
    seq_t *newP = ALLOC_SEQ_ARRAY(new_deg + 1);

    for (size_t i = 0; i <= old_deg; ++i) {
        newP[i] -= P[i];          /* subtract P(x) */
        newP[i + k] += P[i];      /* add P(x) * x^k */
    }

    FREE_SEQ_ARRAY(P);
    *P_ptr = newP;
    *deg_ptr = new_deg;
}

/* Divide polynomial P by (x^k - 1) in place.  The division is exact. */
static void div_by_factor(seq_t **P_ptr, size_t *deg_ptr, idx_t k)
{
    seq_t *P = *P_ptr;
    size_t dP = *deg_ptr;
    size_t dQ = dP - (size_t)k;            /* degree after division */
    seq_t *Q = ALLOC_SEQ_ARRAY(dQ + 1);

    for (size_t n = 0; n <= dQ; ++n) {
        if (n < (size_t)k)
            Q[n] = -P[n];
        else
            Q[n] = Q[n - k] - P[n];
    }

    FREE_SEQ_ARRAY(P);
    *P_ptr = Q;
    *deg_ptr = dQ;
}

/* Compute all coefficients of Φ₁₆₈₀(x).  The array coef[0..384] is filled
   and poly_deg is set to 384. */
static void init_poly(void)
{
    seq_t *P = ALLOC_SEQ_ARRAY(1);   /* start with P(x) = 1 */
    P[0] = 1;
    size_t deg = 0;

    /* Multiply by (x^k - 1) for μ(d) = +1 divisors. */
    for (int i = 0; i < 8; ++i)
        mul_by_factor(&P, &deg, pos_k[i]);

    /* Divide by (x^k - 1) for μ(d) = -1 divisors. */
    for (int i = 0; i < 8; ++i)
        div_by_factor(&P, &deg, neg_k[i]);

    /* Copy coefficients to the static array. */
    for (size_t i = 0; i <= deg; ++i)
        coef[i] = P[i];

    FREE_SEQ_ARRAY(P);
    poly_deg = (idx_t)deg;
}

/* Expand the cache so that it can store values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cached_len > n)
        return;

    size_t need = (size_t)(n + 1);
    if (need <= cached_cap)
        return;

    size_t new_cap = cached_cap ? cached_cap * 2 : 128;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_arr = ALLOC_SEQ_ARRAY(new_cap);
    for (size_t i = 0; i < cached_len; ++i)
        new_arr[i] = cache[i];

    if (cache)
        FREE_SEQ_ARRAY(cache);

    cache = new_arr;
    cached_cap = new_cap;
}

/* External function requested by the build system. */
seq_t a(idx_t n)
{
    static int poly_built = 0;
    if (!poly_built) {
        init_poly();
        poly_built = 1;
    }

    ensure_cache(n);

    /* Initialise first term if cache is empty. */
    if (cached_len == 0) {
        cache[0] = 1;
        cached_len = 1;
    }

    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k <= poly_deg; ++k) {
            if (i < k)   /* guard against underflow, though unlikely */
                break;
            sum = sum + coef[k] * cache[(size_t)(i - k)];
        }
        cache[(size_t)i] = -sum;
    }

    cached_len = (size_t)(n + 1);
    return cache[(size_t)n];
}
