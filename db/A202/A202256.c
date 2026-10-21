/*
 * A202256  Number of zero-sum -n..n arrays of 6 elements with adjacent
 *          element differences also in -n..n.
 * https://oeis.org/A202256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 64, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A202256: Number of zero‑sum –n..n arrays of 6 elements
 * with adjacent element differences also in –n..n.
 *
 * Formula (empirical recurrence):
 *   a(n) = 2*a(n-2)+2*a(n-3) -3*a(n-5)-3*a(n-6)-2*a(n-7)
 *          +a(n-8)+4*a(n-9)+4*a(n-10)+a(n-11)
 *          -2*a(n-12)-3*a(n-13)-3*a(n-14)
 *          +2*a(n-16)+2*a(n-17)-a(n-19)
 *
 * Initial values (n = 1..19):
 *   a(1)=33, a(2)=387, a(3)=2003, a(4)=6963, a(5)=18841,
 *   a(6)=43293, a(7)=88301, a(8)=164873, a(9)=287151,
 *   a(10)=473293, a(11)=745359, a(12)=1130441, a(13)=1660283,
 *   a(14)=2372685, a(15)=3310839, a(16)=4525059, a(17)=6071723,
 *   a(18)=8015439, a(19)=10427561
 *
 * The implementation computes the term iteratively from the first
 * 19 terms.  It uses a temporary array of size n, so it does not
 * require static state and handles calls in arbitrary order.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>  /* for uintptr_t if needed */

/* Return the n‑th term (n >= 1).  Assumes seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Guard against overflow: n is small (<= 10000) in the tests. */
    if (n == 0) return (seq_t)0;   /* undefined, return 0 as safe */
    
    /* Allocate an array of the exact size needed. */
    size_t sz = (size_t)n;
    seq_t *arr = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!arr) return (seq_t)0;     /* allocation failed → return 0 */

    /* Initial 19 values (1‑based indices). */
    static const seq_t init[19] = {
        (seq_t)33,     /* a(1) */
        (seq_t)387,    /* a(2) */
        (seq_t)2003,   /* a(3) */
        (seq_t)6963,   /* a(4) */
        (seq_t)18841,  /* a(5) */
        (seq_t)43293,  /* a(6) */
        (seq_t)88301,  /* a(7) */
        (seq_t)164873, /* a(8) */
        (seq_t)287151, /* a(9) */
        (seq_t)473293, /* a(10) */
        (seq_t)745359, /* a(11) */
        (seq_t)1130441,/* a(12) */
        (seq_t)1660283,/* a(13) */
        (seq_t)2372685,/* a(14) */
        (seq_t)3310839,/* a(15) */
        (seq_t)4525059,/* a(16) */
        (seq_t)6071723,/* a(17) */
        (seq_t)8015439,/* a(18) */
        (seq_t)10427561/* a(19) */
    };

    /* Copy initial values into the array. */
    size_t init_len = (sz < 19) ? sz : 19;
    for (size_t i = 0; i < init_len; ++i)
        arr[i] = init[i];

    /* If n > 19, compute the remaining terms. */
    if (sz > 19) {
        for (idx_t i = 20; i <= n; ++i) {
            /* Compute a(i) using the recurrence.  Index i corresponds to arr[i‑1]. */
            seq_t val = (seq_t)0;
            /* Positive terms */
            val += (seq_t)2 * arr[i-3];  /* 2*a(i-2) */
            val += (seq_t)2 * arr[i-4];  /* 2*a(i-3) */
            val +=          arr[i-9];    /*   a(i-8) */
            val += (seq_t)4 * arr[i-10]; /* 4*a(i-9) */
            val += (seq_t)4 * arr[i-11]; /* 4*a(i-10) */
            val +=          arr[i-12];   /*   a(i-11) */
            val += (seq_t)2 * arr[i-17]; /* 2*a(i-16) */
            val += (seq_t)2 * arr[i-18]; /* 2*a(i-17) */
            /* Negative terms */
            val -= (seq_t)3 * arr[i-6];  /* 3*a(i-5) */
            val -= (seq_t)3 * arr[i-7];  /* 3*a(i-6) */
            val -= (seq_t)2 * arr[i-8];  /* 2*a(i-7) */
            val -= (seq_t)2 * arr[i-13]; /* 2*a(i-12) */
            val -= (seq_t)3 * arr[i-14]; /* 3*a(i-13) */
            val -= (seq_t)3 * arr[i-15]; /* 3*a(i-14) */
            val -=          arr[i-20];   /*   a(i-19) */
            arr[i-1] = val;
        }
    }

    seq_t result = arr[n-1];
    free(arr);
    return result;
}
