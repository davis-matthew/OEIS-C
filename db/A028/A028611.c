/*
 * A028611  Expansion of (theta_3(z)*theta_3(11z) +
 *          theta_2(z)*theta_2(11z))^3.
 * https://oeis.org/A028611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 122, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028611
   Coefficient of q^n in ( θ₃(z)·θ₃(11z) + θ₂(z)·θ₂(11z) )³
   where θ₃(z) = Σ q^{k²},     θ₂(z) = 2 Σ q^{(k+½)²}
   The coefficients are obtained by convolving the expanded
   series of θ₃(z)·θ₃(11z) + θ₂(z)·θ₂(11z) with itself twice.
*/

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Forward declaration for the helper that expands the sequence
   up to 'n'.  C++ handles the vector; C uses raw memory. */
static void ensure_seq_size(idx_t n);

/* Global storage for the coefficients calculated so far.
   They persist between calls to 'a' so that the sequence can
   be evaluated in any order. */
#ifdef __cplusplus
static std::vector<seq_t> a_seq;      /* a(n) */
static std::vector<seq_t> coeff;      /* coefficient of q^n in the inner product   */
static std::vector<seq_t> conv;       /* convolution of coeff with itself          */
#else
static seq_t *a_seq = NULL;   /* a(n) */
static seq_t *coeff = NULL;   /* coefficient array                   */
static seq_t *conv = NULL;    /* convolution array                   */
static size_t a_len = 0;       /* current allocated length            */
#endif

/* Ensure that the internal arrays contain values up to index 'n'.
   This function is called from 'a' and performs the full
   recomputation if a larger 'n' than previously seen is requested. */
static void ensure_seq_size(idx_t n)
{
    /* small utilities used in both languages */
#ifndef __cplusplus
    size_t new_len = (size_t)(n + 1);
    /* Allocate or expand arrays for all three sequences.  */
    a_seq = (seq_t *)realloc(a_seq, new_len * sizeof(seq_t));
    coeff = (seq_t *)realloc(coeff, new_len * sizeof(seq_t));
    conv  = (seq_t *)realloc(conv,  new_len * sizeof(seq_t));
    if (!a_seq || !coeff || !conv) exit(1);
    /* Zero all entries.  */
    for (size_t i = 0; i < new_len; ++i) {
        a_seq[i] = 0;
        coeff[i] = 0;
        conv[i]  = 0;
    }
    a_len = new_len;
#else
    size_t new_len = (size_t)(n + 1);
    a_seq.assign(new_len, 0);
    coeff.assign(new_len, 0);
    conv.assign(new_len, 0);
#endif

    /* ---------------- compute coeff ---------------- */
    /* θ₃(z) · θ₃(11z) part */
    for (idx_t m = 0; m * m <= n; ++m) {
        seq_t c1 = (m == 0 ? 1 : 2);
        idx_t e1 = m * m;
        for (idx_t m2 = 0; e1 + 11 * m2 * m2 <= n; ++m2) {
            seq_t c2 = (m2 == 0 ? 1 : 2);
            idx_t exp = e1 + 11 * m2 * m2;
#ifndef __cplusplus
            coeff[exp] += c1 * c2;
#else
            coeff[exp] += c1 * c2;
#endif
        }
    }

    /* θ₂(z) · θ₂(11z) part */
    for (idx_t n0 = 0;; ++n0) {
        idx_t e0 = n0 * n0 + n0;
        if (e0 > n) break;
        for (idx_t n1 = 0;; ++n1) {
            idx_t e1 = 11 * n1 * n1 + 11 * n1;
            idx_t exp = e0 + e1 + 3;  /* +3 = ¼ + 11·¼   */
            if (exp > n) break;
#ifndef __cplusplus
            coeff[exp] += 4;
#else
            coeff[exp] += 4;
#endif
        }
    }

    /* ---------------- compute conv = coeff * coeff ---------------- */
    for (idx_t i = 0; i <= n; ++i) {
        if (coeff[i] == 0) continue;
        for (idx_t j = 0; j <= n; ++j) {
            idx_t k = i + j;
            if (k > n) break;
#ifndef __cplusplus
            conv[k] += coeff[i] * coeff[j];
#else
            conv[k] += coeff[i] * coeff[j];
#endif
        }
    }

    /* ---------------- compute a_seq = conv * coeff ---------------- */
    for (idx_t i = 0; i <= n; ++i) {
        a_seq[i] = 0;
        for (idx_t k = 0; k <= i; ++k) {
#ifndef __cplusplus
            a_seq[i] += conv[k] * coeff[i - k];
#else
            a_seq[i] += conv[k] * coeff[i - k];
#endif
        }
    }
}

/* Public routine called by the test harness. */
seq_t a(idx_t n)
{
    ensure_seq_size(n);
#ifdef __cplusplus
    return a_seq[(size_t)n];
#else
    return a_seq[(size_t)n];
#endif
}
