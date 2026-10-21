/*
 * A079726  a(1)=1, a(n) is the smallest integer > a(n-1) such that a(n) is
 *          even if k(n)=1, a(n) is odd if k(n)=2, where k(n) denotes the n-th
 *          term of the Kolakoski sequence (A000002).
 * https://oeis.org/A079726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A079726 – Kolakoski‑parity sequence
   a(1) = 1. For n > 1, a(n) is the least integer > a(n‑1)
   having parity even if k(n)=2, odd if k(n)=1,
   where k is the classic Kolakoski sequence (A000002). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised arrays (+ indices) */
    static seq_t  *a_arr   = NULL;   /* a(1)…a(max) */
    static size_t a_cap   = 0;       /* allocated size of a_arr */
    static idx_t  a_curr  = 0;       /* highest index computed so far */

    static unsigned char *k_arr = NULL;   /* Kolakoski terms (1‑based) */
    static size_t         k_cap = 0;
    static idx_t          k_curr = 0;     /* highest index computed so far */

    /* state for generating the Kolakoski sequence */
    static idx_t  run_idx = 2;     /* index of current run‑length in k_arr */
    static unsigned char cur_val = 2; /* value of current run (1 or 2) */
    static idx_t  run_cnt = 1;      /* number of elements of cur_val already used */

    /* On first call, initialise arrays and the first few terms. */
    if (a_curr == 0) {
        /* allocate default capacities */
        a_cap = 16;
        a_arr = (seq_t *)realloc(NULL, a_cap * sizeof(seq_t));

        k_cap = 16;
        k_arr = (unsigned char *)realloc(NULL, k_cap * sizeof(unsigned char));

        /* a(1) */
        a_arr[1] = (seq_t)1;
        a_curr  = 1;

        /* first two Kolakoski terms */
        k_arr[1] = 1;
        k_arr[2] = 2;
        k_curr  = 2;

        /* state for the next run */
        run_idx = 2;        /* second run length has already been used */
        cur_val = 2;        /* value of the second run */
        run_cnt = 1;        /* one element of that run already produced */
    }

    /* grow a_arr if necessary */
    while ((size_t)n + 1 > a_cap) {
        a_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
    }

    /* grow k_arr if necessary */
    while ((size_t)n + 1 > k_cap) {
        k_cap <<= 1;
        k_arr = (unsigned char *)realloc(k_arr, k_cap * sizeof(unsigned char));
    }

    /* generate Kolakoski terms up to index n (if not already done) */
    while (k_curr < n) {
        idx_t pos = k_curr + 1;

        /* finish current run? */
        if (run_cnt == (idx_t)k_arr[run_idx]) {
            run_idx++;               /* move to next run‑length entry */
            cur_val = 3 - cur_val;   /* toggle between 1 and 2 */
            run_cnt = 1;             /* start counting this run anew */
        } else {
            run_cnt++;               /* still inside current run */
        }

        k_arr[pos] = cur_val;
        k_curr = pos;
    }

    /* compute all a values up to n (if needed) */
    while (a_curr < n) {
        idx_t pos  = a_curr + 1;   /* position to compute */
        seq_t prev = a_arr[a_curr];
        seq_t cand = prev + 1;      /* smallest > prev */

        /* enforce required parity from Kolakoski */
        if (k_arr[pos] == 2) {                /* need even */
            if (cand & 1) cand++;             /* bump if odd */
        } else {                              /* need odd */
            if (!(cand & 1)) cand++;          /* bump if even */
        }

        a_arr[pos] = cand;
        a_curr = pos;
    }

    return a_arr[n];
}
