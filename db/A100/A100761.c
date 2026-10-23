/*
 * A100761  Beginning with 1, the smallest positive number which when appended
 *          to the reverse concatenation of all previous terms gives a
 *          multiple of n.
 * https://oeis.org/A100761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n>1, a(n)=min{k>0 | (rev(n-1)·10^len(k)+k) mod n =
 *            0}
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   digits_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A100761.
   For each n ≥ 1 it searches for the smallest positive integer a(n)
   such that the concatenation of a(n) with the reverse of the
   concatenation of all previous terms is divisible by n. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digits_seq.h"

/* Sequence cache -------------------------------------------------------- */
static seq_t *seq_buf = NULL;   /* holds a(i) values (1‑based)   */
static idx_t *len_buf = NULL;   /* decimal digit counts of a(i) (1‑based) */
static idx_t  seq_cnt = 0;      /* number of terms computed   */

/* Ensure terms up to n are computed.  Works for any order of calls. */
static void ensure_terms(idx_t n) {
    if (seq_cnt >= n) return;              /* already computed */

    /* Grow storage arrays if necessary */
    if (seq_buf == NULL) {
        seq_buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        len_buf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    } else if ((n + 1) > seq_cnt) {
        seq_buf = (seq_t *)realloc(seq_buf, (n + 1) * sizeof(seq_t));
        len_buf = (idx_t *)realloc(len_buf, (n + 1) * sizeof(idx_t));
    }

    /* Compute missing terms */
    for (idx_t k = seq_cnt + 1; k <= n; ++k) {
        if (k == 1) {
            seq_buf[1] = (seq_t)1;
            len_buf[1] = 1;
            continue;
        }

        /* Build the reverse concatenation of a(1)…a(k‑1),
           evaluating it modulo k. */
        seq_t rev_mod = 0;
        seq_t mod = (seq_t)k;

        for (idx_t j = k - 1; j > 0; --j) {
            /* Compute 10^(len[j]) mod k */
            seq_t pow10 = 1;
            for (idx_t t = 0; t < len_buf[j]; ++t) {
                pow10 = (pow10 * (seq_t)10) % mod;
            }
            /* a(j) modulo k */
            seq_t val_mod = seq_buf[j] % mod;
            rev_mod = (rev_mod * pow10 + val_mod) % mod;
        }

        /* Search the smallest positive integer d such that
           (rev_mod · 10^len(d) + d) % k == 0. */
        seq_t d = (seq_t)1;
        for (;;) {
            idx_t len_d = digits_seq(d);
            seq_t pow10_d = 1;
            for (idx_t t = 0; t < len_d; ++t) {
                pow10_d = (pow10_d * (seq_t)10) % mod;
            }
            if ((rev_mod * pow10_d + d) % mod == 0) break;
            ++d;
        }

        seq_buf[k] = d;
        len_buf[k] = digits_seq(d);
    }

    seq_cnt = n;
}

/* Public API ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */
    ensure_terms(n);
    return seq_buf[n];
}
