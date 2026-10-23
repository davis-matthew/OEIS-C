/*
 * A119881  Expansion of e.g.f. exp(3*x)*sech(x).
 * https://oeis.org/A119881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} binom(n,2*j) * 3^{n-2*j} * E_{2j}
 *            where E_{2j} are Euler numbers, E_0=1, and recurrence E_{2m} = -
 *            Sum_{k=0}^{m-1} binom(2m,2k) * E_{2k}
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 16, i64 n <= 24, i128 n <= 38,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif
#include "../utils/binom_n_k.h"

/* ----------  static storage  ---------- */
#ifdef __cplusplus
static std::vector<seq_t> a_vals_vec;
static std::vector<seq_t> pow3_vec;
static std::vector<seq_t> euler_vec;
#else
static seq_t *a_vals_ptr = NULL;   /* computed a(n) values          */
static size_t a_size = 0;          /* a_vals_ptr allocated up to a_size-1 */

static seq_t *pow3_ptr = NULL;     /* powers of 3: pow3_ptr[i] = 3^i    */
static size_t pow3_size = 0;       /* pow3_ptr allocated up to pow3_size-1 */

static seq_t *euler_ptr = NULL;    /* euler_ptr[m] = E_{2m}             */
static size_t euler_size = 0;      /* euler_ptr allocated up to euler_size-1 */
#endif

/* ----------  helpers  ---------- */

/* ensure pow3[0..n] exist */
static void
ensure_pow3(idx_t n)
{
#ifdef __cplusplus
    size_t old = pow3_vec.size();
    if (old <= (size_t)n) {
        pow3_vec.resize((size_t)n + 1);
        if (old == 0) {
            pow3_vec[0] = (seq_t)1;
            old = 1;
        }
        for (size_t i = old; i <= (size_t)n; ++i)
            pow3_vec[i] = pow3_vec[i - 1] * (seq_t)3;
    }
#else
    if (pow3_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        pow3_ptr = (seq_t *)realloc(pow3_ptr, new_size * sizeof(seq_t));
        if (!pow3_ptr) abort();
        if (pow3_size == 0) {
            pow3_ptr[0] = (seq_t)1;
            pow3_size = 1;
            if (new_size == 1) return;
        }
        for (size_t i = pow3_size; i <= (size_t)n; ++i)
            pow3_ptr[i] = pow3_ptr[i - 1] * (seq_t)3;
        pow3_size = new_size;
    }
#endif
}

/* ensure euler[0..m] exist   (euler[0] = 1) */
static void
ensure_euler(idx_t m)
{
#ifdef __cplusplus
    size_t old = euler_vec.size();
    if (old <= (size_t)m) {
        euler_vec.resize((size_t)m + 1);
        if (old == 0) {
            euler_vec[0] = (seq_t)1;
            old = 1;
        }
        for (size_t i = old; i <= (size_t)m; ++i) {
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k) {
                seq_t term = binom_n_k((idx_t)(2 * i), (idx_t)(2 * k));
                term = term * euler_vec[k];
                sum += term;
            }
            euler_vec[i] = -sum;
        }
    }
#else
    if (euler_size <= (size_t)m) {
        size_t new_size = (size_t)m + 1;
        euler_ptr = (seq_t *)realloc(euler_ptr, new_size * sizeof(seq_t));
        if (!euler_ptr) abort();
        if (euler_size == 0) {
            euler_ptr[0] = (seq_t)1;
            euler_size = 1;
            if (new_size == 1) return;
        }
        for (size_t i = euler_size; i <= (size_t)m; ++i) {
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k) {
                seq_t term = binom_n_k((idx_t)(2 * i), (idx_t)(2 * k));
                term = term * euler_ptr[k];
                sum += term;
            }
            euler_ptr[i] = -sum;
        }
        euler_size = new_size;
    }
#endif
}

/* ensure a_vals[0..n] exist */
static void
ensure_a(idx_t n)
{
#ifdef __cplusplus
    size_t old = a_vals_vec.size();
    if (old <= (size_t)n) {
        a_vals_vec.resize((size_t)n + 1);
        for (size_t i = old; i <= (size_t)n; ++i) {
            if (i == 0) {
                a_vals_vec[0] = (seq_t)1;
            } else {
                size_t max_j = i / 2;
                ensure_euler((idx_t)max_j);
                ensure_pow3((idx_t)i);
                seq_t sum = 0;
                for (size_t j = 0; j <= max_j; ++j) {
                    seq_t c  = binom_n_k((idx_t)i, (idx_t)(2 * j));
                    seq_t p  = pow3_vec[i - 2 * j];
                    seq_t term = c * p * euler_vec[j];
                    sum += term;
                }
                a_vals_vec[i] = sum;
            }
        }
    }
#else
    if (a_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        a_vals_ptr = (seq_t *)realloc(a_vals_ptr, new_size * sizeof(seq_t));
        if (!a_vals_ptr) abort();
        for (size_t i = a_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                a_vals_ptr[0] = (seq_t)1;
            } else {
                size_t max_j = i / 2;
                ensure_euler((idx_t)max_j);
                ensure_pow3((idx_t)i);
                seq_t sum = 0;
                for (size_t j = 0; j <= max_j; ++j) {
                    seq_t c  = binom_n_k((idx_t)i, (idx_t)(2 * j));
                    seq_t p  = pow3_ptr[i - 2 * j];
                    seq_t term = c * p * euler_ptr[j];
                    sum += term;
                }
                a_vals_ptr[i] = sum;
            }
        }
        a_size = new_size;
    }
#endif
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    ensure_a(n);
#ifdef __cplusplus
    return a_vals_vec[(size_t)n];
#else
    return a_vals_ptr[(size_t)n];
#endif
}
