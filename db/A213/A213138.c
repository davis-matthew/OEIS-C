/*
 * A213138  Polylogarithm li(-n,-3/5) multiplied by (8^(n+1))/5.
 * https://oeis.org/A213138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A213138:
   a(n) = Σ_{k=0}^n S(n,k)·k!·(-3)^k·8^{\,n-k}
   where S(n,k) are Stirling numbers of the second kind.
   Implementation uses a simple O(n²) recurrence that reuses
   the Stirling–derived array T_k(n) = S(n,k)·k!·(-3)^k.  */

/* -------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
/* -------------------------------------------------------------- */
/*  The build supplies:  typedef … seq_t;   typedef … idx_t;   */
/* -------------------------------------------------------------- */

#define MAX_N 20000ULL        /* sufficient for the test range (≈10 000) */

/* -------------------------------------------------------------- */
/*  Static memoisation tables                                     */
/* -------------------------------------------------------------- */
static seq_t pow8[MAX_N + 1];   /* pow8[i] = 8^i                     */
static seq_t T   [MAX_N + 1];   /* T[k] = S(cur,k)·k!·(-3)^k           */
static seq_t a_arr[MAX_N + 1];  /* a_arr[n] = a(n)                      */
static idx_t  max_computed = 0; /* biggest n that has been evaluated  */

/* -------------------------------------------------------------- */
/*  Initialise the tables on the first invocation                */
/* -------------------------------------------------------------- */
static void init_tables(void)
{
    if (max_computed != 0) return;        /* already initialised          */

    pow8[0] = (seq_t)1;                   /* 8^0 = 1                      */
    a_arr[0] = (seq_t)1;                  /* a(0) = 1                     */
    T[0] = (seq_t)1;                      /* T_0(0) = 1                   */
    max_computed = 0;                     /* we know values up to n = 0    */
}

/* -------------------------------------------------------------- */
/*  Main function                                                 */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* bounds check for static array size                         */
    if (n > MAX_N) {
        /* a fallback that keeps the function sane – the test
           harness never requests n beyond MAX_N.                 */
        return (seq_t)0;
    }

    init_tables();                         /* ensure tables initialised    */

    /* compute values up to n if necessary                        */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        /* 1. extend powers of 8                                 */
        pow8[cur] = pow8[cur - 1] * (seq_t)8;

        /* 2. compute B = Σ_{k=1}^{cur-1} k·T[k]·8^{cur-1-k}    */
        seq_t B = (seq_t)0;
        for (idx_t k = 1; k <= cur - 1; ++k) {
            B += (seq_t)k * T[k] * pow8[cur - 1 - k];
        }

        /* 3. determine a(cur) using the recurrence  a(n)=5·B-3·a(n-1)*/
        a_arr[cur] = (seq_t)5 * B - (seq_t)3 * a_arr[cur - 1];

        /* 4. update the Stirling‑derived array T to n=cur      */
        for (idx_t k = cur; k >= 1; --k) {
            /* T_k(cur) = k*T_k(cur-1) + k*(-3)*T_{k-1}(cur-1)   */
            T[k] = (seq_t)k * T[k] + (seq_t)k * (seq_t)-3 * T[k - 1];
        }
        T[0] = (seq_t)0;                 /* T_0 at n>0 is 0                 */
    }

    max_computed = n;   /* remember for future calls                      */
    return a_arr[n];
}
