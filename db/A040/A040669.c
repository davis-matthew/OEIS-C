/*
 * A040669  Continued fraction for sqrt(696).
 * https://oeis.org/A040669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = continued fraction coefficient n of sqrt(696). *
 *            Computation: use the standard recurrence for quadratic
 *            irrationals: * m_{k+1} = d_k a_k – m_k * d_{k+1} = (D –
 *            m_{k+1}²) / d_k * a_{k+1} = ⌊ (a0 + m_{k+1}) / d_{k+1} ⌋ * where
 *            D = 696, a0 = ⌊√D⌋ = 26. The sequence is stored in a * cache so
 *            t...
 * Range:     u8 n <= 0, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cached coefficients of the continued fraction of √696. */
static seq_t *cf_cache   = NULL;   /* array of coefficients */
static size_t cf_len     = 0;       /* number of elements currently cached */
static size_t cf_cap     = 0;       /* allocated capacity of cf_cache */
static bool   cf_ready   = false;  /* cache initialisation flag */

static const seq_t D = (seq_t)696;      /* the square‑free part */
static const seq_t a0_const = (seq_t)26;/* floor(√696) */

/* State for generating the next coefficient.  These are updated
   every time a new term is produced.  The state corresponds to
   m_k, d_k, a_k in the recurrence. */
static seq_t m = (seq_t)0;
static seq_t d = (seq_t)1;
static seq_t a_k = (seq_t)26; /* a0 is already stored */

/* ------------------------------------------------------------------ */
/* Ensure that cf_cache has room for indices up to 'need'.  The cache
   is grown by doubling its size each time.  Memcpy is not needed
   because we only append elements. */
static void cf_ensure_capacity(idx_t need)
{
    if ((size_t)need < cf_len) return;
    size_t new_cap = cf_cap ? cf_cap : 16;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cf_cache, new_cap * sizeof(seq_t));
    cf_cache = tmp;
    cf_cap   = new_cap;
}

/* Expand cf_cache until it contains terms up to 'need' inclusive. */
static void cf_expand_to(idx_t need)
{
    if (!cf_ready) {
        cf_ready = true;
        m = (seq_t)0;
        d = (seq_t)1;
        a_k = a0_const;
        cf_len = 0;
    }
    cf_ensure_capacity(need);
    while ((idx_t)cf_len <= need) {
        if (cf_len == 0) {
            /* first term is a0 */
            cf_cache[0] = a0_const;
        } else {
            /* compute next term using 64‑bit intermediates */
            unsigned long long m_next = (unsigned long long)d * (unsigned long long)a_k -
                                       (unsigned long long)m;
            unsigned long long d_next = ((unsigned long long)D -
                                         m_next * m_next) /
                                         (unsigned long long)d;
            unsigned long long a_next = ((unsigned long long)a0_const + m_next) /
                                         d_next;
            /* store the computed term */
            cf_cache[cf_len] = (seq_t)a_next;
            /* update state for next iteration */
            m = (seq_t)m_next;
            d = (seq_t)d_next;
            a_k = (seq_t)a_next;
        }
        cf_len++;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the test harness.  It returns the
   n‑th coefficient of the continued fraction of √696. */
seq_t a(idx_t n)
{
    cf_expand_to(n);
    return cf_cache[n];
}
