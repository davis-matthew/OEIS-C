/*
 * A087149  Least positive multiples of index n that can result from the
 *          self-convolution of a monotonically increasing sequence (A087148).
 * https://oeis.org/A087149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087149 – least positive multiples of index n that can result
   from the self-convolution of a monotonically increasing sequence
   (the sequence A087148).  For each n>=1 let
          S(n) = Σ_{k=1}^{n} b(k)*b(n-k+1)
   where b is the minimal increasing sequence satisfying
          n | S(n).
   Then a(n) = S(n)/n.
   The function computes b(n) recursively: for n>1,
          S(n) = 2*b(n) + const,
          const = Σ_{k=2}^{n-1} b(k)*b(n-k+1).
   We find the smallest b(n) > b(n-1) such that 2*b(n)+const ≡ 0 (mod n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                    /* offset starts at 1 */

    /* Cached values */
    static seq_t *b = NULL;       /* 1..cur  : values of the A087148 sequence */
    static seq_t *A = NULL;       /* 1..cur  : values of this sequence */
    static idx_t cur = 0;         /* highest index that has been computed */
    static size_t cap = 0;        /* allocated capacity of the arrays */

    /* Initialisation */
    if (cur == 0) {
        cap = 4;                               /* small initial size */
        b  = (seq_t *)malloc(cap * sizeof(seq_t));
        A  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!b || !A) return (seq_t)0;         /* allocation failure */
        b[1] = (seq_t)1;   /* b(1) = 1 */
        A[1] = (seq_t)1;   /* a(1) = 1 */
        cur = 1;
    }

    /* Compute values up to n */
    while (cur < n) {
        idx_t i = cur + 1;            /* next index to compute */

        /* Grow arrays if necessary */
        if (i >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap <= i) new_cap = i + 1;
            b = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
            A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (!b || !A) return (seq_t)0;
            cap = new_cap;
        }

        /* Compute const = Σ_{k=2}^{i-1} b(k)*b(i-k+1) and its remainder mod i */
        seq_t const_full = (seq_t)0;             /* full integer value */
        seq_t const_mod  = (seq_t)0;             /* value modulo i (seq_t) */
        for (idx_t k = 2; k <= i - 1; ++k) {
            seq_t prod = b[k] * b[i - k + 1];
            const_full = const_full + prod;
            const_mod  = (const_mod + prod) % i; /* i is idx_t, implicit cast */
        }
        /* const_mod fits into idx_t */
        idx_t const_mod_i = (idx_t)const_mod;

        /* Solve 2*b(i) ≡ -const (mod i) for minimal positive b(i) */
        seq_t b_i;         /* temporary value for b(i) */
        idx_t step;        /* step size for consecutive solutions */
        idx_t residue;     /* base residue in [0,i-1] */

        if (i & 1U) {            /* odd i – invert 2 modulo i */
            idx_t inv2 = (i + 1) / 2;          /* inverse of 2 modulo i */
            idx_t tmp  = (i - const_mod_i) % i; /* -const mod i */
            residue    = (tmp * inv2) % i;      /* solution modulo i */
            step       = i;
        } else {                 /* even i – solve mod i/2 */
            idx_t m = i >> 1;                 /* i/2 */
            /* const_mod_i must be even */
            idx_t half = const_mod_i >> 1;    /* const / 2 modulo m */
            idx_t tmp  = (m - half % m) % m;   /* -half mod m */
            residue    = tmp;                  /* solution modulo m */
            step       = m;                    /* step size m */
            if (residue == 0) residue = m;     /* ensure positivity */
        }

        /* Convert residue to seq_t for comparison/computation */
        b_i = (seq_t)residue;
        /* Ensure b_i > b[i-1] */
        while (b_i <= b[i - 1]) {
            b_i = b_i + step;      /* step is idx_t, implicit conversion */
        }

        /* Store b(i) */
        b[i] = b_i;

        /* Compute a(i) = (2*b(i)+const_full)/i */
        seq_t total = (seq_t)2 * b_i + const_full;
        A[i] = total / (seq_t)i;   /* clean division */

        cur = i;
    }

    return A[n];
}
