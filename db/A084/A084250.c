/*
 * A084250  Let a(1) = 1; for n>1, let a(n) be smallest positive integer
 *          distinct from all earlier terms such that exp(Sum_{n>=1}
 *          a(n)*x^n/n) has integer coefficients (cf. A084251).
 * https://oeis.org/A084250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A084250
 *  a(1) = 1.
 *  For n>1, a(n) is the smallest unused positive integer such that
 *  the exponential generating function 
 *      exp( Σ_{k≥1} a(k) x^k / k )
 *  has integer coefficients.  Equivalently, if b(0)=1 and
 *      n * b(n) = Σ_{k=1..n} a(k) * b(n-k)   (n≥1),
 *  then a(n) must satisfy a(n) ≡ -Σ_{k=1..n-1} a(k)*b(n-k) (mod n).
 *  The function below builds the sequence lazily, caching all
 *  computed a(n) and the auxiliary b(n) values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t idx_t;   /* assume idx_t is unsigned 64‑bit */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static arrays for a(n) and b(n).  a[1..len] is used. */
    static seq_t *a_seq = NULL;     /* a[1..len]   */
    static seq_t *b_seq = NULL;     /* b[0..len]   */
    static size_t a_cap = 0;        /* capacity of a_seq */
    static size_t b_cap = 0;        /* capacity of b_seq */
    static size_t a_len = 0;        /* number of a terms computed (≥0) */

    /* Initialise on first call. */
    if (a_seq == NULL) {
        a_cap = 64;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_cap = 65;
        b_seq = (seq_t *)malloc(b_cap * sizeof(seq_t));

        /* a(1) = 1, b(0) = 1, b(1) = a(1) / 1 = 1 */
        a_seq[1] = (seq_t)1;
        b_seq[0] = (seq_t)1;
        b_seq[1] = (seq_t)1;
        a_len   = 1;
    }

    /* Extend cache until we have a(n). */
    while (a_len < (size_t)n) {
        idx_t new_n = (idx_t)(a_len + 1);   /* next term to compute */

        /* Compute S = Σ_{k=1..new_n-1} a(k) * b(new_n-k).   */
        seq_t S = (seq_t)0;     /* full value, used for b(new_n) later */
        seq_t S_mod = (seq_t)0; /* reduced modulo new_n for congruence */

        for (idx_t k = 1; k <= new_n - 1; ++k) {
            seq_t prod = a_seq[k] * b_seq[new_n - k];
            S += prod;

            seq_t prod_mod = (a_seq[k] % (seq_t)new_n) * (b_seq[new_n - k] % (seq_t)new_n);
            S_mod = (S_mod + prod_mod) % (seq_t)new_n;
        }

        /* Determine the required congruence class: a_n ≡ r (mod new_n) */
        seq_t r = ((seq_t)new_n - (S_mod % (seq_t)new_n)) % (seq_t)new_n;
        seq_t candidate = (r == 0 ? (seq_t)new_n : r);  /* if r==0 congruence class 0 */

        /* Find the smallest unused candidate in that congruence class */
        bool used;
        do {
            used = false;
            for (size_t i = 1; i <= a_len; ++i) {
                if (a_seq[i] == candidate) { used = true; break; }
            }
            if (used)
                candidate += (seq_t)new_n;
        } while (used);

        /* Now compute b(new_n) = (S + candidate) / new_n  (exact division) */
        seq_t b_new = (S + candidate) / (seq_t)new_n;

        /* Ensure capacity */
        if (a_len + 1 >= a_cap) {
            a_cap *= 2;
            a_seq = (seq_t *)realloc(a_seq, a_cap * sizeof(seq_t));
        }
        if (a_len + 1 >= b_cap) {
            b_cap *= 2;
            b_seq = (seq_t *)realloc(b_seq, b_cap * sizeof(seq_t));
        }

        /* Store new values */
        a_seq[a_len + 1] = candidate;
        b_seq[a_len + 1] = b_new;
        ++a_len;
    }

    return a_seq[(size_t)n];
}
